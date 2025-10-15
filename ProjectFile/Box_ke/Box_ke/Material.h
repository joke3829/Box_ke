// ==================================================
// Material.h
// 객체의 재질 정보를 담고 있는 클래스
// ==================================================

#pragma once
#include "stdafx.h"

struct PhongMaterialCbuffer {
	XMFLOAT4	diffuseColor{};
	XMFLOAT4	specularColor{};
	XMFLOAT4	ambientColor{};
	XMFLOAT4	emissiveColor{};
	float		shininess{};
	float		padding[3];
};

class CMaterial {
public:
	void SetStartSlot(UINT slot);
	void SetRootParameterIndex(UINT index);

	virtual void SetShaderVariable(void* command, ShaderType type = ST_NONE) {}
protected:
	union {
		UINT					m_StartSlot{};
		UINT					m_RootParameterIndex;
	};
};

class CPhongShadingMaterial : public CMaterial {
public:
	CPhongShadingMaterial(PhongMaterialCbuffer material);

	PhongMaterialCbuffer& GetMaterial();
protected:
	PhongMaterialCbuffer		m_Material;
};

// DX11 =================================================================

class CPhongShadingMaterialDX11 : public CPhongShadingMaterial {
public:
	CPhongShadingMaterialDX11(ID3D11Device* device, PhongMaterialCbuffer material);

	void UpdateMaterialBuffer(ID3D11DeviceContext* command);
	void SetShaderVariable(void* command, ShaderType type = ST_NONE);
protected:
	ComPtr<ID3D11Buffer>			m_MaterialBuffer{};
};