#pragma once
#include "stdafx.h"
#include "Light.h"

constexpr UINT MAX_LIGHTS = 100;

struct ShaderLightCB {
	LightCB lights[MAX_LIGHTS];
	UINT numLights{};
};

class CLightManager {
public:
	virtual void AddDirectionalLight(XMFLOAT4 lightColor, XMFLOAT3 direction, float intensity) {}
	virtual void AddSpotLight(XMFLOAT3 pos, XMFLOAT4 lightColor, XMFLOAT3 direction, float intensity, float range, float spotAngle) {}
	virtual void AddPointLight(XMFLOAT3 pos, XMFLOAT4 lightColor, float intensity, float range) {}

	virtual void UpdateLights(float elapsedTime);
	virtual void SetShaderVariable(void* command, ShaderType type = ST_NONE) {}

	void SetStartSlot(UINT slot);
	void SetRootParameterIndex(UINT index);

	std::shared_ptr<CLight>& GetLightWithIndex(size_t index);
protected:
	UINT						m_numDirAndSpotLight{};
	UINT						m_numPointLight{};

	union {
		UINT					m_StartSlot{};
		UINT					m_RootParameterIndex;
	};

	std::vector<LightCB>		m_LightCBInfo{};
	std::vector<std::shared_ptr<CLight>>			m_Lights{};
};

class CLightManagerDX11 : public CLightManager {
public:
	CLightManagerDX11(ID3D11Device* device);

	void AddDirectionalLight(XMFLOAT4 lightColor, XMFLOAT3 direction, float intensity);
	void AddSpotLight(XMFLOAT3 pos, XMFLOAT4 lightColor, XMFLOAT3 direction, float intensity, float range, float spotAngle);
	void AddPointLight(XMFLOAT3 pos, XMFLOAT4 lightColor, float intensity, float range);

	//void UpdateLights(float elapsedTime);
	void SetShaderVariable(void* command, ShaderType type = ST_NONE);
protected:
	void UpdateConstantBuffer(ID3D11DeviceContext* context);

	ID3D11Device*										m_Device{};
	ComPtr<ID3D11Buffer>								m_LightConstantBuffer{};
};