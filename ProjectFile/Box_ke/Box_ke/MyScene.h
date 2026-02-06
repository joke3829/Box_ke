#pragma once
#include "Scene.h"
#include "MyObject.h"
#include "BloomProcessor.h"
#include "LightManager.h"

// Example
class CTestSceneDX11 : public CScene {
public:
	void Setup(void* device, void* command = nullptr);

	void ProcessInput(float elapsedTime);
	void KeyboardMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void MouseMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void UpdateObject(float elapsedTime, void* command = nullptr);

	void Render(void* command = nullptr);
protected:
	std::unique_ptr<CCamera>		m_Camera{};
	std::unique_ptr<CShader>		m_Shader{};

	D3D11_VIEWPORT					m_Viewport{};
	std::vector<std::shared_ptr<CGameObject>>	m_Objects{};

	POINT							oldCursor{};
	bool							click{};

	ID3D11Device* tempdev{};
};

class CShaderTestSceneDX11 : public CScene {
public:
	void Setup(void* device, void* command = nullptr);

	void ProcessInput(float elapsedTime);
	void KeyboardMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void MouseMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void UpdateObject(float elapsedTime, void* command = nullptr);

	void Render(void* command = nullptr);
protected:
	std::unique_ptr<CCamera>		m_Camera{};
	std::unique_ptr<CShader>		m_Shader{};

	D3D11_VIEWPORT					m_Viewport{};
	std::vector<std::shared_ptr<CGameObject>>	m_Objects{};

	POINT							oldCursor{};
	bool							click{};

	ID3D11Device* tempdev{};
};

// ========================================================

struct ClientSizeCBuffer {
	UINT width;
	UINT height;
};

class CDeferredRenderSceneDX11 : public CScene{
public:
	CDeferredRenderSceneDX11(ComPtr<ID3D11Device4> device, ComPtr<IDXGISwapChain4> swapChain, 
		ComPtr<ID3D11RenderTargetView> rtv, ComPtr<ID3D11DepthStencilView> dsv, 
		UINT& clientWidth, UINT& clientHeight);

	void ProcessInput(float elapsedTime);
	void KeyboardMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void MouseMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void UpdateObject(float elapsedTime, void* command = nullptr);

	void PreRender(void* command = nullptr);
	void Render(void* command = nullptr);
	void PostRender(void* command = nullptr);
protected:
	void ResizeTargets();
	void CreateTargets();
	void BuildObjects();

	void OnePathRender(ID3D11DeviceContext* context);
	void TwoPathRender(ID3D11DeviceContext* context);
	UINT&							m_ClientWidth;
	UINT&							m_ClientHeight;

	ComPtr<ID3D11Device4>			m_Device{};
	ComPtr<IDXGISwapChain4>			m_SwapChain{};

	ComPtr<ID3D11RenderTargetView>	m_MainRTV{};
	ComPtr<ID3D11DepthStencilView>	m_MainDSV{};

	// Define_RenderTarget.pdf 참고
	std::vector<ComPtr<ID3D11RenderTargetView>>		m_RTVs{};
	std::vector<ComPtr<ID3D11ShaderResourceView>>	m_SRVs{};

	std::shared_ptr<CShader>						m_MRTShader{};
	std::shared_ptr<CShader>						m_RenderShader{};
	std::shared_ptr<CShader>						m_TextureRenderShader{};
	std::shared_ptr<CShader>						m_ToneMappingShader{};

	std::shared_ptr<CCamera>						m_Camera{};
	std::vector<std::shared_ptr<CGameObject>>		m_Objects{};

	ComPtr<ID3D11Texture2D>							m_OutputTexture{};
	ComPtr<ID3D11ShaderResourceView>				m_OutputSRV{};
	ComPtr<ID3D11RenderTargetView>					m_OutputRTV{};
	// Bloom
	std::shared_ptr<CBloomProcessor>				m_BloomProcessor{};

	D3D11_VIEWPORT									m_Viewport{};
	POINT							oldCursor{};
	bool							click{};

	bool							m_OnBloom{ true };

	// Light
	std::shared_ptr<CLightManager>					m_LightManager{};

	bool animationonoff{};
};