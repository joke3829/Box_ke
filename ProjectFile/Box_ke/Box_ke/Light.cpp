#include "Light.h"

CLight::CLight(LightCB& cb, XMFLOAT4 lightColor, XMFLOAT3 direction, float intensity, UINT index)
	: m_LightInfo{ cb }
{
	cb.type = LT_DIRECTIONAL;
	cb.lightColor = lightColor;
	cb.direction = direction;
	cb.intensity = intensity;
	cb.index = index;

	m_InitialDir = direction;
	// temp
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(DEFAULT_SHADOWMAP_SIZE, DEFAULT_SHADOWMAP_SIZE, 0.001f, 500.f));

	XMStoreFloat4x4(&m_LocalMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
}

CLight::CLight(LightCB& cb, XMFLOAT3 pos, XMFLOAT4 lightColor, XMFLOAT3 direction, float intensity, float range, float spotAngle, UINT index)
	: m_LightInfo{ cb }
{
	cb.type = LT_SPOT;
	cb.position = pos;
	cb.lightColor = lightColor;
	cb.direction = direction;
	cb.intensity = intensity;
	cb.range = range;
	cb.spotAngle = spotAngle;
	cb.index = index;

	m_Position = pos;
	m_InitialDir = direction;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixPerspectiveFovLH(spotAngle, 1.f, 0.001f, range));

	XMStoreFloat4x4(&m_LocalMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
}

CLight::CLight(LightCB& cb, XMFLOAT3 pos, XMFLOAT4 lightColor, float intensity, float range, UINT index)
	: m_LightInfo{ cb }
{
	cb.type = LT_POINT;
	cb.position = pos;
	cb.lightColor = lightColor;
	cb.intensity = intensity;
	cb.range = range;
	cb.index = index;

	m_Position = pos;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixPerspectiveFovLH(90.f, 1.f, 0.001f, range));

	XMStoreFloat4x4(&m_LocalMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
}

void CLight::SetParentObject(CGameObject* target)
{
	m_ParentObject = target;
}

void CLight::UpdateLightInfo()
{
	m_LightInfo.position = { m_WorldMatrix._41, m_WorldMatrix._42, m_WorldMatrix._43 };
	if (m_LightInfo.type != LT_POINT) {
		XMStoreFloat3(&m_LightInfo.direction, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_InitialDir), XMLoadFloat4x4(&m_WorldMatrix))));
	}
}

void CLight::UpdateMatrix()	// 부정확
{
	m_LocalMatrix._11 = m_Right.x; m_LocalMatrix._12 = m_Right.y; m_LocalMatrix._13 = m_Right.z; m_LocalMatrix._14 = 0;
	m_LocalMatrix._21 = m_Up.x; m_LocalMatrix._22 = m_Up.y; m_LocalMatrix._23 = m_Up.z; m_LocalMatrix._24 = 0;
	m_LocalMatrix._31 = m_Look.x; m_LocalMatrix._32 = m_Look.y; m_LocalMatrix._33 = m_Look.z; m_LocalMatrix._34 = 0;
	m_LocalMatrix._41 = m_Position.x; m_LocalMatrix._42 = m_Position.y; m_LocalMatrix._43 = m_Position.z; m_LocalMatrix._44 = 1;
	if (m_ParentObject) {
		XMFLOAT4X4 parent = m_ParentObject->GetWorldMatrix();
		XMStoreFloat4x4(&m_WorldMatrix, XMLoadFloat4x4(&m_LocalMatrix) * XMLoadFloat4x4(&parent));
	}
	else
		m_WorldMatrix = m_LocalMatrix;

	UpdateLightInfo();
}

// =======================================================================================

CLightDX11::CLightDX11(ID3D11Device* device, LightCB& cb, XMFLOAT4 lightColor, XMFLOAT3 direction, float intensity, ID3D11Texture2D* gShadowMap, UINT index)
	: CLight(cb, lightColor, direction, intensity, index)
{
	// temp
	m_Viewport = { 0, 0, DEFAULT_SHADOWMAP_SIZE, DEFAULT_SHADOWMAP_SIZE, 0.f, 1.f };
	//XMStoreFloat4x4(&m_ProjMatrix, XMMatrixPerspectiveFovLH(XMConvertToRadians(90.f), 1.f, 0.01f, range)); orthoGraphic으로 변경
	ReadyElements(device, gShadowMap);
}

CLightDX11::CLightDX11(ID3D11Device* device, LightCB& cb, XMFLOAT3 pos, XMFLOAT4 lightColor, XMFLOAT3 direction, float intensity, float range, float spotAngle, ID3D11Texture2D* gShadowMap, UINT index)
	: CLight(cb, pos, lightColor, direction, intensity, range, spotAngle, index)
{
	m_Viewport = { 0, 0, DEFAULT_SHADOWMAP_SIZE, DEFAULT_SHADOWMAP_SIZE, 0.f, 1.f };
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixPerspectiveFovLH(XMConvertToRadians(90.f), 1.f, 0.01f, range));
	ReadyElements(device, gShadowMap);
}

CLightDX11::CLightDX11(ID3D11Device* device, LightCB& cb, XMFLOAT3 pos, XMFLOAT4 lightColor, float intensity, float range, ID3D11Texture2D* gShadowMap, UINT index)
	: CLight(cb, pos, lightColor, intensity, range, index)
{
	m_Viewport = { 0, 0, DEFAULT_SHADOWMAP_SIZE, DEFAULT_SHADOWMAP_SIZE, 0.f, 1.f };
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixPerspectiveFovLH(XMConvertToRadians(90.f), 1.f, 0.01f, range));
	ReadyElements(device, gShadowMap, true);
}

void CLightDX11::ReadyElements(ID3D11Device* device, ID3D11Texture2D* gShadowMap, bool bPointLight)
{
	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = DEFAULT_SHADOWMAP_SIZE;
	desc.Height = DEFAULT_SHADOWMAP_SIZE;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.SampleDesc.Count = 1;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	// DepthStencil
	ComPtr<ID3D11Texture2D> tempBuffer{};
	HRESULT hResult = device->CreateTexture2D(&desc, nullptr, tempBuffer.GetAddressOf());
	if (not SUCCEEDED(hResult)) {
		MessageBoxA(0, "Failed Light DepthBuffer", "Fatal Error", MB_OK);
		PostQuitMessage(0);
	}
	device->CreateDepthStencilView(tempBuffer.Get(), nullptr, m_DepthStencilView.GetAddressOf());

	/*desc.Width = DEFAULT_SHADOWMAP_SIZE;
	desc.Height = DEFAULT_SHADOWMAP_SIZE;
	desc.ArraySize = bPointLight ? 6 : 1;
	desc.Format = DXGI_FORMAT_R32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.MiscFlags = bPointLight ? D3D11_RESOURCE_MISC_TEXTURECUBE : 0;

	hResult = device->CreateTexture2D(&desc, nullptr, m_ShadowMap.GetAddressOf());
	if (not SUCCEEDED(hResult)) {
		MessageBoxA(0, "Failed Light DepthBuffer", "Fatal Error", MB_OK);
		PostQuitMessage(0);
	}

	if (bPointLight) {
		D3D11_SHADER_RESOURCE_VIEW_DESC sdesc{};
		sdesc.Format = DXGI_FORMAT_R32_FLOAT;
		sdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		sdesc.TextureCube.MipLevels = 1;
		hResult = device->CreateShaderResourceView(m_ShadowMap.Get(), &sdesc, m_ShadowMapSRV.GetAddressOf());
	}
	else
		hResult = device->CreateShaderResourceView(m_ShadowMap.Get(), nullptr, m_ShadowMapSRV.GetAddressOf());

	if (not SUCCEEDED(hResult)) {
		MessageBoxA(0, "Failed CreateSRV Section Light", "Fatal Error", MB_OK);
		PostQuitMessage(0);
	}*/

	D3D11_RENDER_TARGET_VIEW_DESC rtv{};
	rtv.Format = DXGI_FORMAT_R32_FLOAT;
	rtv.Texture2DArray.ArraySize = 1;
	rtv.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;

	// ShadowMap RenderTarget
	if (bPointLight) {
		for (int i = 0; i < 6; ++i) {
			rtv.Texture2DArray.FirstArraySlice = (m_LightInfo.index * 6) + i;
			ComPtr<ID3D11RenderTargetView> temp{};
			hResult = device->CreateRenderTargetView(gShadowMap, &rtv, temp.GetAddressOf());
			if (not SUCCEEDED(hResult)) {
				MessageBoxA(0, "Failed Light RTV", "Fatal Error", MB_OK);
				PostQuitMessage(0);
			}
			m_ShadowMapRTVs.push_back(temp);
		}
	}
	else {
		rtv.Texture2DArray.FirstArraySlice = m_LightInfo.index;
		ComPtr<ID3D11RenderTargetView> temp{};
		hResult = device->CreateRenderTargetView(gShadowMap, &rtv, temp.GetAddressOf());
		if (not SUCCEEDED(hResult)) {
			MessageBoxA(0, "Failed Light RTV", "Fatal Error", MB_OK);
			PostQuitMessage(0);
		}
		m_ShadowMapRTVs.push_back(temp);
	}

	D3D11_BUFFER_DESC bdesc{};
	bdesc.ByteWidth = Align(sizeof(XMFLOAT4X4), 16);
	bdesc.Usage = D3D11_USAGE_DYNAMIC;
	bdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	device->CreateBuffer(&bdesc, nullptr, m_ViewProjBuffer.GetAddressOf());

	// test 26.01.21
	bdesc.ByteWidth = Align(sizeof(LightCB), 16);
	device->CreateBuffer(&bdesc, nullptr, m_cbLightBuffer.GetAddressOf());
}

void CLightDX11::UpdateMatrix()
{
	m_LocalMatrix._11 = m_Right.x; m_LocalMatrix._12 = m_Right.y; m_LocalMatrix._13 = m_Right.z; m_LocalMatrix._14 = 0;
	m_LocalMatrix._21 = m_Up.x; m_LocalMatrix._22 = m_Up.y; m_LocalMatrix._23 = m_Up.z; m_LocalMatrix._24 = 0;
	m_LocalMatrix._31 = m_Look.x; m_LocalMatrix._32 = m_Look.y; m_LocalMatrix._33 = m_Look.z; m_LocalMatrix._34 = 0;
	m_LocalMatrix._41 = m_Position.x; m_LocalMatrix._42 = m_Position.y; m_LocalMatrix._43 = m_Position.z; m_LocalMatrix._44 = 1;
	if (m_ParentObject) {
		CHierarchyGameObjectDX11* p = dynamic_cast<CHierarchyGameObjectDX11*>(m_ParentObject);
		XMFLOAT4X4 parent;
		if (p) parent = p->GetHierarchyWorldMat();
		else parent = m_ParentObject->GetWorldMatrix();
		XMStoreFloat4x4(&m_WorldMatrix, XMLoadFloat4x4(&m_LocalMatrix) * XMLoadFloat4x4(&parent));
	}
	else
		m_WorldMatrix = m_LocalMatrix;

	UpdateLightInfo();
}

void CLightDX11::UpdateShadowMap(void* command, std::vector<std::shared_ptr<CGameObject>>& objects, bool useCameraPos, XMFLOAT3 cameraPos)
{
	ID3D11DeviceContext* context = reinterpret_cast<ID3D11DeviceContext*>(command);

	switch (m_LightInfo.type) {
	case LT_POINT: {
		float clearColor[4] = { 1.f, 0.f, 0.f, 0.f };
		context->RSSetViewports(1, &m_Viewport);
		XMVECTOR eye = XMLoadFloat3(&m_LightInfo.position);
		XMMATRIX proj = XMLoadFloat4x4(&m_ProjMatrix);
		XMMATRIX view;
		XMVECTOR dir[6] = {
			{1.f, 0.f, 0.f, 0.f},
			{-1.f, 0.f, 0.f, 0.f},
			{0.f, 1.f, 0.f, 0.f},
			{0.f, -1.f, 0.f, 0.f},
			{0.f, 0.f, 1.f, 0.f},
			{0.f, 0.f, -1.f, 0.f}
		};

		XMVECTOR up[6] = {
			{0.f, 1.f, 0.f, 0.f},
			{0.f, 1.f, 0.f, 0.f},
			{0.f, 0.f,-1.f, 0.f},
			{0.f, 0.f, 1.f, 0.f},
			{0.f, 1.f, 0.f, 0.f},
			{0.f, 1.f, 0.f, 0.f}
		};
		/*for (int i = 0; i < 6; ++i) {
			view = XMMatrixLookToLH(eye, dir[i], up[i]);
			XMStoreFloat4x4(&m_LightInfo.ShadowMapMatrix[i], XMMatrixTranspose(view * proj));
		}*/
		for (int i = 0; i < 6; ++i) {
			context->OMSetRenderTargets(1, m_ShadowMapRTVs[i].GetAddressOf(), m_DepthStencilView.Get());
			context->ClearRenderTargetView(m_ShadowMapRTVs[i].Get(), clearColor);
			context->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

			D3D11_MAPPED_SUBRESOURCE mapped{};
			context->Map(m_ViewProjBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
			view = XMMatrixLookToLH(eye, dir[i], up[i]);
			XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(mapped.pData), XMMatrixTranspose(view * proj));
			//memcpy(mapped.pData, &m_LightInfo.ShadowMapMatrix[i], sizeof(XMFLOAT4X4));
			context->Unmap(m_ViewProjBuffer.Get(), 0);

			context->Map(m_cbLightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
			memcpy(mapped.pData, &m_LightInfo, sizeof(LightCB));
			context->Unmap(m_cbLightBuffer.Get(), 0);

			context->VSSetConstantBuffers(1, 1, m_ViewProjBuffer.GetAddressOf());
			context->PSSetConstantBuffers(3, 1, m_cbLightBuffer.GetAddressOf());

			for (auto& p : objects) {
				p->Render(context);
			}
		}
		break;
	}
	case LT_SPOT: {
		break;
	}
	case LT_DIRECTIONAL: {
		break;
	}
	}
}

void CLightDX11::UpdateLight(float elapsedTime, void* command, std::vector<std::shared_ptr<CGameObject>>& objects)
{
	UpdateMatrix();
	UpdateShadowMap(command, objects);	// 추가 할거면 하고
}

void CLightDX11::UpdateLightInfo()
{
	m_LightInfo.position = { m_WorldMatrix._41, m_WorldMatrix._42, m_WorldMatrix._43 };
	switch (m_LightInfo.type) {
	case LT_POINT: {
		/*XMVECTOR eye = XMLoadFloat3(&m_LightInfo.position);
		XMMATRIX proj = XMLoadFloat4x4(&m_ProjMatrix);
		XMMATRIX view;
		XMVECTOR dir[6] = {
			{1.f, 0.f, 0.f, 0.f},
			{-1.f, 0.f, 0.f, 0.f},
			{0.f, 1.f, 0.f, 0.f},
			{0.f, -1.f, 0.f, 0.f},
			{0.f, 0.f, 1.f, 0.f},
			{0.f, 0.f, -1.f, 0.f}
		};

		XMVECTOR up[6] = {
			{0.f, 1.f, 0.f, 0.f},
			{0.f, 1.f, 0.f, 0.f},
			{0.f, 0.f,-1.f, 0.f},
			{0.f, 0.f, 1.f, 0.f},
			{0.f, 1.f, 0.f, 0.f},
			{0.f, 1.f, 0.f, 0.f}
		};
		for (int i = 0; i < 6; ++i) {
			view = XMMatrixLookToLH(eye, dir[i], up[i]);
			XMStoreFloat4x4(&m_LightInfo.ShadowMapMatrix[i], XMMatrixTranspose(view * proj));
		}*/
		break;
	}
	default: {
		XMStoreFloat3(&m_LightInfo.direction, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_InitialDir), XMLoadFloat4x4(&m_WorldMatrix))));
		break;
	}
	}
}

//ID3D11ShaderResourceView* CLightDX11::GetShadowMapSRV()
//{
//	return m_ShadowMapSRV.Get();
//}