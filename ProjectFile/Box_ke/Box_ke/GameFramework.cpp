// ===========================================================
// GameFramework.cpp
// Framework 함수들의 정의를 정리해둔 cpp 파일
// ===========================================================
#include "GameFramework.h"

void CGameFramework::WMMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
		m_Scene->KeyboardMessageProcessing(hWnd, message, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
		m_Scene->MouseMessageProcessing(hWnd, message, wParam, lParam);
		break;
	}
}

void CGameFramework::KeyboardMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

}

void CGameFramework::MouseMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

}

// DX11 Framework ========================================================

bool CGameFrameworkDX11::Initialize(HWND hWnd, UINT clientWidth, UINT clientHeight)
{
	m_hWnd = hWnd;
	m_clientWidth = clientWidth; m_clientHeight = clientHeight;

	m_LimitedFrame = 120.f;

	if (!InitDevice()) return false;
	if (!InitSwapChain()) return false;
	if (!InitRTVDSV()) return false;

	m_Scene = std::make_shared<CDeferredRenderSceneDX11>(m_Device, m_SwapChain, 
		m_RerderTargetView, m_DepthStencilView, m_clientWidth, m_clientHeight);
	//m_Scene = std::make_shared<CShaderTestSceneDX11>();
	m_Scene->Setup(m_Device.Get(), m_DeviceContext.Get());

	m_Timer = std::make_shared<CTimer>();

	return true;
}

bool CGameFrameworkDX11::InitDevice()
{
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	D3D_FEATURE_LEVEL features[] = { D3D_FEATURE_LEVEL_11_1 ,D3D_FEATURE_LEVEL_11_0 };
	HRESULT hResult = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, 0, features, 2, D3D11_SDK_VERSION,
		&device, nullptr, &context);
	if (FAILED(hResult)) {
		MessageBoxA(0, "Failed D3D11CreateDevice()", "Fatal Error", MB_OK);
		return false;
	}

	device->QueryInterface(IID_PPV_ARGS(m_Device.GetAddressOf()));
	context->QueryInterface(IID_PPV_ARGS(m_DeviceContext.GetAddressOf()));

	device->Release();
	context->Release();
	return true;
}

bool CGameFrameworkDX11::InitSwapChain()
{
	ComPtr<IDXGIFactory7>	factory;
	ComPtr<IDXGIAdapter>	adapter;
	ComPtr<IDXGIDevice4>	dxgiDevice;

	m_Device->QueryInterface(IID_PPV_ARGS(dxgiDevice.GetAddressOf()));

	dxgiDevice->GetParent(IID_PPV_ARGS(adapter.GetAddressOf()));

	adapter->GetParent(IID_PPV_ARGS(factory.GetAddressOf()));
	ComPtr<IDXGISwapChain1> swapchain{};

	DXGI_SWAP_CHAIN_DESC1 desc{};
	desc.Width = 0;		// window Size
	desc.Height = 0;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 2;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hResult = factory->CreateSwapChainForHwnd(m_Device.Get(), m_hWnd, &desc, nullptr, nullptr, swapchain.GetAddressOf());

	if (FAILED(hResult)) {
		MessageBoxA(0, "Failed CreateSwapChainForHwnd()", "Fatal Error", MB_OK);
		return false;
	}

	swapchain->QueryInterface(IID_PPV_ARGS(m_SwapChain.GetAddressOf()));

	return true;
}

bool CGameFrameworkDX11::InitRTVDSV()
{
	ComPtr<ID3D11Texture2D> buffer;
	m_SwapChain->GetBuffer(0, IID_PPV_ARGS(buffer.GetAddressOf()));

	HRESULT hResult = m_Device->CreateRenderTargetView(buffer.Get(), nullptr, m_RerderTargetView.GetAddressOf());
	if (FAILED(hResult)) {
		MessageBoxA(0, "Failed CreateRenderTargetView()", "Fatal Error", MB_OK);
		return false;
	}

	ComPtr<ID3D11Texture2D> DepthStencilBuffer{};
	D3D11_TEXTURE2D_DESC ddesc{};
	buffer->GetDesc(&ddesc);
	ddesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	ddesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	hResult = m_Device->CreateTexture2D(&ddesc, 0, DepthStencilBuffer.GetAddressOf());
	if (FAILED(hResult)) {
		MessageBoxA(0, "Failed CreateDepthStencil Buffer", "Fatal Error", MB_OK);
		return false;
	}

	m_Device->CreateDepthStencilView(DepthStencilBuffer.Get(), nullptr, m_DepthStencilView.GetAddressOf());
	if (FAILED(hResult)) {
		MessageBoxA(0, "Failed CreateDepthStencilView()", "Fatal Error", MB_OK);
		return false;
	}

	return true;
}

void CGameFrameworkDX11::Render()
{
	float ElapsedTime = m_Timer->Tick(m_LimitedFrame);
	int tt = m_Timer->getFPS();

	float clearColor[] = { 0.0, 0.0, 0.0, 1.0 };
	m_DeviceContext->ClearRenderTargetView(m_RerderTargetView.Get(), clearColor);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_DeviceContext->OMSetRenderTargets(1, m_RerderTargetView.GetAddressOf(), m_DepthStencilView.Get());

	m_Scene->ProcessInput(ElapsedTime);
	m_Scene->UpdateObject(ElapsedTime, m_DeviceContext.Get());

	m_Scene->PreRender(m_DeviceContext.Get());
	m_Scene->Render(m_DeviceContext.Get());
	m_Scene->PostRender(m_DeviceContext.Get());

	m_SwapChain->Present(0, 0);

	std::wstring frame{ std::to_wstring(tt) + L"FPS"};
	SetWindowText(m_hWnd, frame.c_str());
}

// =======================================================================


// DX12 ==================================================================

bool CGameFrameworkDX12::Initialize(HWND hWnd, UINT clientWidth, UINT clientHeight)
{
	return true;
}

bool CGameFrameworkDX12::InitDevice()
{
	return true;
}

bool CGameFrameworkDX12::InitSwapChain()
{
	return true;
}

void CGameFrameworkDX12::Render()
{

}

// =======================================================================