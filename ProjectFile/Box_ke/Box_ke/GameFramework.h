// ==========================================================
// GameFramework.h
// ���������ӿ�ũ�� DX �������� ��Ƶ� ���
// 12�� ���� �߰� ����
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
	float							m_LimitedFrame{};			// ������ ����, 0.0f�� �� ���� ����

	std::shared_ptr<CScene>			m_Scene{};					// Scene���� ī�޶� ������?
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
