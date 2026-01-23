#pragma once
#include "stdafx.h"
#include "Object.h"
struct alignas(16) LightCB {
	UINT type{};
	XMFLOAT3 position{};
	XMFLOAT4 lightColor{};
	XMFLOAT3 direction{};
	float intensity{};
	float range{};
	float spotAngle{};
	UINT index{};		// shader에서 사용할 shadowmap 인덱스, point일 경우 texturecube에 대한 인덱스, 이외에는 texture2D에 대한 인덱스
	float padding{};
	XMFLOAT4X4 ShadowMapMatrix{};		// DirectionalLight만 이용할 예정
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

	virtual void UpdateLightInfo();
	virtual void UpdateMatrix();
	virtual void UpdateLight(float elapsedTime) {}		// like animation
	virtual void UpdateShadowMap(void* command, std::vector<std::shared_ptr<CGameObject>>& objects, bool useCameraPos = false, XMFLOAT3 cameraPos = {}) {}	// Directional Light

	// Animation + matrix + shadowMap Update
	virtual void UpdateLight(float elapsedTime, void* command, std::vector<std::shared_ptr<CGameObject>>& objects) {}	
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
	CLightDX11(ID3D11Device* device, LightCB& cb, XMFLOAT4 lightColor, XMFLOAT3 direction, float intensity, ID3D11Texture2D* gShadowMap, UINT index);		// Directional Light
	CLightDX11(ID3D11Device* device, LightCB& cb, XMFLOAT3 pos, XMFLOAT4 lightColor, XMFLOAT3 direction, float intensity, float range, float spotAngle, ID3D11Texture2D* gShadowMap, UINT index);	// Spot Light
	CLightDX11(ID3D11Device* device, LightCB& cb, XMFLOAT3 pos, XMFLOAT4 lightColor, float intensity, float range, ID3D11Texture2D* gShadowMap, UINT index);	// Point Light

	virtual void UpdateMatrix();
	void UpdateShadowMap(void* command, std::vector<std::shared_ptr<CGameObject>>& objects, bool useCameraPos = false, XMFLOAT3 cameraPos = {});

	// Animation + matrix + shadowMap Update
	void UpdateLight(float elapsedTime, void* command, std::vector<std::shared_ptr<CGameObject>>& objects);
	void UpdateLightInfo();

	//ID3D11ShaderResourceView* GetShadowMapSRV();
protected:
	void ReadyElements(ID3D11Device* device, ID3D11Texture2D* gShadowMap, bool bPointLight = false);

	D3D11_VIEWPORT									m_Viewport{};

	//ComPtr<ID3D11Texture2D>							m_DepthStencilBuffer{};
	ComPtr<ID3D11DepthStencilView>					m_DepthStencilView{};

	std::vector<ComPtr<ID3D11RenderTargetView>>		m_ShadowMapRTVs{};

	// 26.01.23 global resource를 쓸거기에 주석 처리
	//ComPtr<ID3D11Texture2D>							m_ShadowMap{};
	//ComPtr<ID3D11ShaderResourceView>				m_ShadowMapSRV{};

	ComPtr<ID3D11Buffer>							m_ViewProjBuffer{};
	
	// test 26.01.21
	ComPtr<ID3D11Buffer>							m_cbLightBuffer{};
};