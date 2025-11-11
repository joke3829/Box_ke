#include "LightManager.h"

// 시간의 따른 조명 변화와 행렬(LightInfo) 최신화
void CLightManager::UpdateLights(float elapsedTime)
{ 
	for (auto& p : m_Lights) {
		p->UpdateLight(elapsedTime);
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
	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = Align(sizeof(LightCB) * MAX_LIGHTS, 16);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

	device->CreateBuffer(&desc, nullptr, m_LightConstantBuffer.GetAddressOf());
}

void CLightManagerDX11::AddDirectionalLight(XMFLOAT4 lightColor, XMFLOAT3 direction, float intensity)
{
	m_LightCBInfo.emplace_back();
	m_Lights.emplace_back(std::make_shared<CLightDX11>(m_Device, m_LightCBInfo.back(), lightColor, direction, intensity, m_numDirAndSpotLight++));
}

void CLightManagerDX11::AddSpotLight(XMFLOAT3 pos, XMFLOAT4 lightColor, XMFLOAT3 direction, float intensity, float range, float spotAngle)
{
	m_LightCBInfo.emplace_back();
	m_Lights.emplace_back(std::make_shared<CLightDX11>(m_Device, m_LightCBInfo.back(), pos, lightColor, direction, intensity, range, spotAngle, m_numDirAndSpotLight++));
}

void CLightManagerDX11::AddPointLight(XMFLOAT3 pos, XMFLOAT4 lightColor, float intensity, float range)
{
	m_LightCBInfo.emplace_back();
	m_Lights.emplace_back(std::make_shared<CLightDX11>(m_Device, m_LightCBInfo.back(), pos, lightColor, intensity, range, m_numPointLight++));
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

void CLightManagerDX11::UpdateConstantBuffer(ID3D11DeviceContext* context)
{
	D3D11_MAPPED_SUBRESOURCE mapped{};
	context->Map(m_LightConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	memcpy(mapped.pData, m_LightCBInfo.data(), sizeof(LightCB) * (m_numDirAndSpotLight + m_numPointLight));
	context->Unmap(m_LightConstantBuffer.Get(), 0);
}