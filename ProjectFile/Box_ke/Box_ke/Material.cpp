#include "Material.h"

void CMaterial::SetStartSlot(UINT slot)
{
	m_StartSlot = slot;
}

void CMaterial::SetRootParameterIndex(UINT index)
{
	m_RootParameterIndex = index;
}

CPhongShadingMaterial::CPhongShadingMaterial(PhongMaterialCbuffer material)
	: m_Material{ material }
{
}

PhongMaterialCbuffer& CPhongShadingMaterial::GetMaterial()
{
	return m_Material;
}

// DX11 =================================================================

CPhongShadingMaterialDX11::CPhongShadingMaterialDX11(ID3D11Device* device, PhongMaterialCbuffer material)
	: CPhongShadingMaterial(material)
{
	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = Align(sizeof(PhongMaterialCbuffer), 16);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = &m_Material;
	device->CreateBuffer(&desc, &data, m_MaterialBuffer.GetAddressOf());
}

void CPhongShadingMaterialDX11::UpdateMaterialBuffer(ID3D11DeviceContext* command)
{
	D3D11_MAPPED_SUBRESOURCE data{};
	command->Map(m_MaterialBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	memcpy(data.pData, &m_Material, sizeof(PhongMaterialCbuffer));
	command->Unmap(m_MaterialBuffer.Get(), 0);
}

void CPhongShadingMaterialDX11::SetShaderVariable(void* command, ShaderType type)
{
	ID3D11DeviceContext* context = reinterpret_cast<ID3D11DeviceContext*>(command);
	switch(type) {
	case ST_VS:
		context->VSSetConstantBuffers(m_StartSlot, 1, m_MaterialBuffer.GetAddressOf());
		break;
	case ST_HS:
		context->HSSetConstantBuffers(m_StartSlot, 1, m_MaterialBuffer.GetAddressOf());
		break;
	case ST_DS:
		context->DSSetConstantBuffers(m_StartSlot, 1, m_MaterialBuffer.GetAddressOf());
		break;
	case ST_GS:
		context->GSSetConstantBuffers(m_StartSlot, 1, m_MaterialBuffer.GetAddressOf());
		break;
	case ST_PS:
		context->PSSetConstantBuffers(m_StartSlot, 1, m_MaterialBuffer.GetAddressOf());
		break;
	case ST_CS:
		context->CSSetConstantBuffers(m_StartSlot, 1, m_MaterialBuffer.GetAddressOf());
		break;
	default:
		MessageBoxA(0, "Insert Invalid Type", "Fatal Error_DX11", MB_OK);
		break;
	}
}