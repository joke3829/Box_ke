#pragma once
#include "stdafx.h"
#include "Light.h"
#include "MyShader.h"

constexpr UINT MAX_LIGHTS = 100;

struct ShaderLightCB {
	LightCB lights[MAX_LIGHTS]{};
	UINT numLights{};
};

class CLightManager {
public:
	virtual void AddDirectionalLight(XMFLOAT4 lightColor, XMFLOAT3 direction, XMFLOAT3 up, float intensity) {}
	virtual void AddSpotLight(XMFLOAT3 pos, XMFLOAT4 lightColor, XMFLOAT3 direction, XMFLOAT3 up, float intensity, float range, float spotAngle) {}
	virtual void AddPointLight(XMFLOAT3 pos, XMFLOAT4 lightColor, float intensity, float range) {}

	virtual void UpdateLights(float elapsedTime);
	virtual void UpdateLights(float elapsedTime, void* command, std::vector<std::shared_ptr<CGameObject>>& objects, CCamera* camera = nullptr) {}	// Update With ShadowMap
	virtual void SetShaderVariable(void* command, ShaderType type = ST_NONE) {}
	virtual void SetShadowMapTextureShaderVariable(void* command, ShaderType type = ST_NONE) {}
	virtual void SetShadowMapShader(void* command) {}
	// 01.09
	//virtual void SetShadowMapTexture(void* command) {}

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

	std::shared_ptr<CShader>	m_ShadowMapShader{};
};

class CLightManagerDX11 : public CLightManager {
public:
	CLightManagerDX11(ID3D11Device* device);

	void AddDirectionalLight(XMFLOAT4 lightColor, XMFLOAT3 direction, XMFLOAT3 up, float intensity);
	void AddSpotLight(XMFLOAT3 pos, XMFLOAT4 lightColor, XMFLOAT3 direction, XMFLOAT3 up, float intensity, float range, float spotAngle);
	void AddPointLight(XMFLOAT3 pos, XMFLOAT4 lightColor, float intensity, float range);

	// Directional을 위해 메인 카메라의 정보를 넘겨주게 수정하라
	void UpdateLights(float elapsedTime, void* command, std::vector<std::shared_ptr<CGameObject>>& objects, CCamera* camera = nullptr);	// Update With ShadowMap

	//void UpdateLights(float elapsedTime);
	void SetShaderVariable(void* command, ShaderType type = ST_NONE);
	void SetShadowMapTextureShaderVariable(void* command, ShaderType type = ST_NONE);
	void SetShadowMapShader(void* command);
protected:
	void UpdateConstantBuffer(ID3D11DeviceContext* context);

	ID3D11Device*										m_Device{};
	ComPtr<ID3D11Buffer>								m_LightConstantBuffer{};

	//ID3D11ShaderResourceView*							m_ShadowMapSRVs[50]{};
	//ID3D11ShaderResourceView*							m_ShadowMapCubeSRVs[50]{};

	// 26.01.23 global shadowmap array를 이용해본다.
	ComPtr<ID3D11Texture2D>								m_ShadowMapArray{};
	ComPtr<ID3D11Texture2D>								m_ShadowMapCubeArray{};

	ComPtr<ID3D11ShaderResourceView>					m_ShadowMapArraySRV{};
	ComPtr<ID3D11ShaderResourceView>					m_ShadowMapCubeArraySRV{};

};