#include "LightManager.h"

// 시간의 따른 조명 변화와 행렬(LightInfo) 최신화
void CLightManager::UpdateLights(float elapsedTime)
{ 
	std::vector<std::shared_ptr<CGameObject>> temp{};
	for (auto& p : m_Lights) {
		p->UpdateLight(elapsedTime, nullptr, temp);
		p->UpdateMatrix();
	}
}

void CLightManager::SetStartSlot(UINT slot)
{
	m_StartSlot = slot;
}

void CLightManager::SetRootParameterIndex(UINT index)
{
	m_RootParameterIndex = index;
}

std::shared_ptr<CLight>& CLightManager::GetLightWithIndex(size_t index)
{
	return m_Lights[index];
}

// DX11 =====================================================================================

CLightManagerDX11::CLightManagerDX11(ID3D11Device* device)
	: m_Device{ device }
{
	HRESULT hResult;

	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = Align(sizeof(ShaderLightCB), 16);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

	hResult = device->CreateBuffer(&desc, nullptr, m_LightConstantBuffer.GetAddressOf());
	if (not SUCCEEDED(hResult)) {
		MessageBoxA(0, "Failed Create LightConstantBuffer - LightManager", "Fatal Error", MB_OK);
		PostQuitMessage(0);
	}

	m_LightCBInfo.reserve(MAX_LIGHTS);
	m_Lights.reserve(MAX_LIGHTS);

	m_ShadowMapShader = std::make_shared<CShadowMapShaderDX11>(device);

	UINT numCubeLight = MAX_LIGHTS / 2;

	// 26.01.23 global ShadowMap Ready, SRV Ready
	D3D11_TEXTURE2D_DESC tdesc{};
	tdesc.Width = DEFAULT_SHADOWMAP_SIZE;
	tdesc.Height = DEFAULT_SHADOWMAP_SIZE;
	tdesc.ArraySize = numCubeLight * 6;
	tdesc.Format = DXGI_FORMAT_R32_FLOAT;
	tdesc.SampleDesc.Count = 1;
	tdesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	tdesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	hResult = device->CreateTexture2D(&tdesc, nullptr, m_ShadowMapCubeArray.GetAddressOf());
	if (not SUCCEEDED(hResult)) {
		MessageBoxA(0, "Failed Create ShadowMapCubeArray - LightManager", "Fatal Error", MB_OK);
		PostQuitMessage(0);
	}

	tdesc.ArraySize = MAX_LIGHTS - numCubeLight;
	tdesc.MiscFlags = 0;
	hResult = device->CreateTexture2D(&tdesc, nullptr, m_ShadowMapArray.GetAddressOf());
	if (not SUCCEEDED(hResult)) {
		MessageBoxA(0, "Failed Create ShadowMapArray - LightManager", "Fatal Error", MB_OK);
		PostQuitMessage(0);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC vdesc{};
	vdesc.Format = DXGI_FORMAT_R32_FLOAT;
	vdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
	vdesc.TextureCubeArray.MipLevels = 1;
	vdesc.TextureCubeArray.NumCubes = numCubeLight;
	hResult = device->CreateShaderResourceView(m_ShadowMapCubeArray.Get(), &vdesc, m_ShadowMapCubeArraySRV.GetAddressOf());
	if (not SUCCEEDED(hResult)) {
		MessageBoxA(0, "Failed Create CubeArraySRV - LightManager", "Fatal Error", MB_OK);
		PostQuitMessage(0);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC sdesc{};
	sdesc.Format = DXGI_FORMAT_R32_FLOAT;
	sdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	sdesc.Texture2DArray.ArraySize = MAX_LIGHTS - numCubeLight;
	sdesc.Texture2DArray.MipLevels = 1;
	hResult = device->CreateShaderResourceView(m_ShadowMapArray.Get(), &sdesc, m_ShadowMapArraySRV.GetAddressOf());
	if (not SUCCEEDED(hResult)) {
		MessageBoxA(0, "Failed Create ArraySRV - LightManager", "Fatal Error", MB_OK);
		PostQuitMessage(0);
	}
}

void CLightManagerDX11::AddDirectionalLight(XMFLOAT4 lightColor, XMFLOAT3 direction, XMFLOAT3 up, float intensity)
{
	m_LightCBInfo.emplace_back();
	std::shared_ptr<CLightDX11> temp = std::make_shared<CLightDX11>(m_Device, m_LightCBInfo.back(), lightColor, direction, up, intensity, m_ShadowMapArray.Get(), m_numDirAndSpotLight++);
	m_Lights.emplace_back(temp);
	//m_ShadowMapSRVs[m_numDirAndSpotLight++] = temp->GetShadowMapSRV();
}

void CLightManagerDX11::AddSpotLight(XMFLOAT3 pos, XMFLOAT4 lightColor, XMFLOAT3 direction, XMFLOAT3 up, float intensity, float range, float spotAngle)
{
	m_LightCBInfo.emplace_back();
	std::shared_ptr<CLightDX11> temp = std::make_shared<CLightDX11>(m_Device, m_LightCBInfo.back(), pos, lightColor, direction, up, intensity, range, spotAngle, m_ShadowMapArray.Get(), m_numDirAndSpotLight++);
	m_Lights.emplace_back(temp);
	//m_ShadowMapSRVs[m_numDirAndSpotLight++] = temp->GetShadowMapSRV();
}

void CLightManagerDX11::AddPointLight(XMFLOAT3 pos, XMFLOAT4 lightColor, float intensity, float range)
{
	m_LightCBInfo.emplace_back();
	std::shared_ptr<CLightDX11> temp = std::make_shared<CLightDX11>(m_Device, m_LightCBInfo.back(), pos, lightColor, intensity, range, m_ShadowMapCubeArray.Get(), m_numPointLight++);
	m_Lights.emplace_back(temp);
	//m_ShadowMapCubeSRVs[m_numPointLight++] = temp->GetShadowMapSRV();
}

void CLightManagerDX11::UpdateLights(float elapsedTime, void* command, std::vector<std::shared_ptr<CGameObject>>& objects, CCamera* camera)
{
	// shadowMap Shader Set
	m_ShadowMapShader->SetShader(command);

	for (auto& p : m_Lights) { // include ShadowMap Update
		p->UpdateLight(elapsedTime, command, objects, camera);
	}
}

void CLightManagerDX11::SetShaderVariable(void* command, ShaderType type)
{
	ID3D11DeviceContext* context = reinterpret_cast<ID3D11DeviceContext*>(command);
	UpdateConstantBuffer(context);

	switch (type) {
	case ST_VS:
		context->VSSetConstantBuffers(m_StartSlot, 1, m_LightConstantBuffer.GetAddressOf());
		break;
	case ST_HS:
		context->HSSetConstantBuffers(m_StartSlot, 1, m_LightConstantBuffer.GetAddressOf());
		break;
	case ST_DS:
		context->DSSetConstantBuffers(m_StartSlot, 1, m_LightConstantBuffer.GetAddressOf());
		break;
	case ST_GS:
		context->GSSetConstantBuffers(m_StartSlot, 1, m_LightConstantBuffer.GetAddressOf());
		break;
	case ST_PS:
		context->PSSetConstantBuffers(m_StartSlot, 1, m_LightConstantBuffer.GetAddressOf());
		break;
	case ST_CS:
		context->CSSetConstantBuffers(m_StartSlot, 1, m_LightConstantBuffer.GetAddressOf());
		break;
	default:
		MessageBoxA(0, "Insert Invalid Type", "Fatal Error_DX11", MB_OK);
		break;
	}
}

void CLightManagerDX11::SetShadowMapTextureShaderVariable(void* command, ShaderType type)
{
	ID3D11DeviceContext* context = reinterpret_cast<ID3D11DeviceContext*>(command);

	switch (type) {
	case ST_VS:
		context->VSSetShaderResources(7, 1, m_ShadowMapCubeArraySRV.GetAddressOf());
		context->VSSetShaderResources(8, 1, m_ShadowMapArraySRV.GetAddressOf());
		break;
	case ST_HS:
		context->HSSetConstantBuffers(m_StartSlot, 1, m_LightConstantBuffer.GetAddressOf());
		break;
	case ST_DS:
		context->DSSetConstantBuffers(m_StartSlot, 1, m_LightConstantBuffer.GetAddressOf());
		break;
	case ST_GS:
		context->GSSetConstantBuffers(m_StartSlot, 1, m_LightConstantBuffer.GetAddressOf());
		break;
	case ST_PS:
		context->PSSetShaderResources(7, 1, m_ShadowMapCubeArraySRV.GetAddressOf());
		context->PSSetShaderResources(8, 1, m_ShadowMapArraySRV.GetAddressOf());
		break;
	case ST_CS:
		context->CSSetConstantBuffers(m_StartSlot, 1, m_LightConstantBuffer.GetAddressOf());
		break;
	default:
		MessageBoxA(0, "Insert Invalid Type", "Fatal Error_DX11", MB_OK);
		break;
	}
}

void CLightManagerDX11::SetShadowMapShader(void* command)
{
	m_ShadowMapShader->SetShader(command);
}

void CLightManagerDX11::UpdateConstantBuffer(ID3D11DeviceContext* context)
{
	D3D11_MAPPED_SUBRESOURCE mapped{};
	UINT numLights = m_numPointLight + m_numDirAndSpotLight;
	context->Map(m_LightConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	ShaderLightCB* p = reinterpret_cast<ShaderLightCB*>(mapped.pData);
	memcpy(p->lights, m_LightCBInfo.data(), sizeof(LightCB) * (numLights));
	p->numLights = numLights;
	context->Unmap(m_LightConstantBuffer.Get(), 0);
}