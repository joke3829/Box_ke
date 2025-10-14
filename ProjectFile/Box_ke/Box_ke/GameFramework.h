// ==========================================================
// GameFramework.h
// 게임프레임워크를 DX 버젼별로 모아둔 헤더
// 12는 추후 추가 예정
// ===========================================================
#pragma once
#include "stdafx.h"
#include "Timer.h"
#include "Camera.h"
#include "MyScene.h"

// Base Class=======================================================
class CGameFramework {
public:
	virtual bool Initialize(HWND hWnd, UINT clientWidth, UINT clientHeight) { return false; }

	virtual void WMMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual void KeyboardMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual void MouseMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void Render() {};
protected:
	std::shared_ptr<CTimer>			m_Timer{};
	float							m_LimitedFrame{};			// 프레임 제한, 0.0f일 시 제한 없음

	std::shared_ptr<CScene>			m_Scene{};					// Scene별로 카메라를 가질까?
};


// DX11 Framework ==================================================
class CGameFrameworkDX11 : public CGameFramework {
public:
	bool Initialize(HWND hWnd, UINT clientWidth, UINT clientHeight);

	void Render();
private:
	bool InitDevice();
	bool InitSwapChain();
	bool InitRTVDSV();

	UINT							m_clientWidth{};
	UINT							m_clientHeight{};

	HWND							m_hWnd{};

	ComPtr<ID3D11Device4>			m_Device{};
	ComPtr<ID3D11DeviceContext4>	m_DeviceContext{};

	ComPtr<IDXGISwapChain4>			m_SwapChain{};
	
	ComPtr<ID3D11RenderTargetView>	m_RerderTargetView{};
	ComPtr<ID3D11DepthStencilView>	m_DepthStencilView{};
};



// DX12 Framework ==================================================
class CGameFrameworkDX12 : public CGameFramework {
public:
	bool Initialize(HWND hWnd, UINT clientWidth, UINT clientHeight);
	void Render();
private:
	bool InitDevice();
	bool InitSwapChain();
};
