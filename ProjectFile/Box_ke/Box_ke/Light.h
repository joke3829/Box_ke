#pragma once
#include "stdafx.h"
#include "Object.h"
struct LightCB {
	UINT type;
	XMFLOAT3 position;
	XMFLOAT4 lightColor;
	XMFLOAT3 direction;
	float intensity;
	float range;
	float spotAngle;
	UINT index;		// shader에서 사용할 shadowmap 인덱스, point일 경우 texturecube에 대한 인덱스, 이외에는 texture2D에 대한 인덱스
	float padding;
	XMFLOAT4X4 ShadowMapMatrix[6];
};

constexpr UINT DEFAULT_SHADOWMAP_SIZE = 1024;

class CLight {
public:
	CLight(LightCB& cb, XMFLOAT4 lightColor, XMFLOAT3 direction, float intensity, UINT index);		// Directional Light
	CLight(LightCB& cb, XMFLOAT3 pos, XMFLOAT4 lightColor, XMFLOAT3 direction, float intensity, float range, float spotAngle, UINT index);	// Spot Light
	CLight(LightCB& cb, XMFLOAT3 pos, XMFLOAT4 lightColor, float intensity, float range, UINT index);	// Point Light

	void SetParentObject(CGameObject* target);
	// ============================
	void SetPosition() {}
	void Rotate() {}
	// ============================

	void UpdateLightInfo();
	virtual void UpdateMatrix();
	virtual void UpdateLight(float elapsedTime) {}		// like animation
	virtual void UpdateShadowMap(void* command, bool useCameraPos = false, XMFLOAT3 cameraPos = {}) {}
protected:
	LightCB&				m_LightInfo;
	CGameObject*			m_ParentObject{};

	XMFLOAT4X4				m_WorldMatrix{};
	XMFLOAT4X4				m_LocalMatrix{};
	XMFLOAT4X4				m_ProjMatrix{};

	XMFLOAT3				m_Right{ 1.f, 0.f, 0.f };
	XMFLOAT3				m_Up{ 0.f, 1.f, 0.f };
	XMFLOAT3				m_Look{ 0.f, 0.f, 1.f };

	XMFLOAT3				m_Position{};
	XMFLOAT3				m_InitialDir{};
};

class CLightDX11 : public CLight {
public:
	CLightDX11(ID3D11Device* device, LightCB& cb, XMFLOAT4 lightColor, XMFLOAT3 direction, float intensity, UINT index);		// Directional Light
	CLightDX11(ID3D11Device* device, LightCB& cb, XMFLOAT3 pos, XMFLOAT4 lightColor, XMFLOAT3 direction, float intensity, float range, float spotAngle, UINT index);	// Spot Light
	CLightDX11(ID3D11Device* device, LightCB& cb, XMFLOAT3 pos, XMFLOAT4 lightColor, float intensity, float range, UINT index);	// Point Light

	virtual void UpdateMatrix();
	void UpdateShadowMap(void* command, bool useCameraPos = false, XMFLOAT3 cameraPos = {});
protected:
	D3D11_VIEWPORT									m_Viewport{};

	ComPtr<ID3D11Texture2D>							m_DepthStencilBuffer{};
	ComPtr<ID3D11DepthStencilView>					m_DepthStencilView{};

	ComPtr<ID3D11Texture2D>							m_ShadowMap{};
	std::vector<ComPtr<ID3D11RenderTargetView>>		m_ShadowMapRTVs{};
};