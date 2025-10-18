#include "MyShader.h"

CShaderDX11DrawTriangleShader::CShaderDX11DrawTriangleShader(ID3D11Device* device)
{
	CreateVSAndInputLayout(device);
	CreatePS(device);
}

void CShaderDX11DrawTriangleShader::ShaderReCompile(void* device)
{
	ID3D11Device* td = reinterpret_cast<ID3D11Device*>(device);

	CreateVSAndInputLayout(td);
	CreatePS(td);
}

void CShaderDX11DrawTriangleShader::CreateVSAndInputLayout(ID3D11Device* device)
{
	ComPtr<ID3DBlob> pBlob = CompileHLSL(L"Shaders\\TestShader1.hlsl", "vsmain", "vs_5_0");
	HRESULT hResult = device->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, m_VS.GetAddressOf());

	if (FAILED(hResult)) {
		MessageBoxA(0, "Failed CreateVertexShader()", "Fatal Error", MB_OK);
		PostQuitMessage(0);
	}

	D3D11_INPUT_ELEMENT_DESC descs[]{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	hResult = device->CreateInputLayout(descs, 1, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), m_InputLayout.GetAddressOf());
	if (FAILED(hResult)) {
		MessageBoxA(0, "Failed CreateInputLayout()", "Fatal Error", MB_OK);
		PostQuitMessage(0);
	}
}

void CShaderDX11DrawTriangleShader::CreatePS(ID3D11Device* device)
{
	ComPtr<ID3DBlob> pBlob = CompileHLSL(L"Shaders\\TestShader1.hlsl", "psmain", "ps_5_0");
	HRESULT hResult = device->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, m_PS.GetAddressOf());

	if (FAILED(hResult)) {
		MessageBoxA(0, "Failed CreatePixelShader()", "Fatal Error", MB_OK);
		PostQuitMessage(0);
	}
}



// ====================================================================

CShaderDX11TestShader::CShaderDX11TestShader(ID3D11Device* device)
{
	CreateVSAndInputLayout(device);
	CreatePS(device);
}

void CShaderDX11TestShader::ShaderReCompile(void* device)
{
	ID3D11Device* td = reinterpret_cast<ID3D11Device*>(device);

	CreateVSAndInputLayout(td);
	CreatePS(td);
}

void CShaderDX11TestShader::CreateVSAndInputLayout(ID3D11Device* device)
{
	ComPtr<ID3DBlob> pBlob = CompileHLSL(L"Shaders\\TestShader1.hlsl", "vsTest", "vs_5_0");
	HRESULT hResult = device->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, m_VS.GetAddressOf());

	if (FAILED(hResult)) {
		MessageBoxA(0, "Failed CreateVertexShader()", "Fatal Error", MB_OK);
		PostQuitMessage(0);
	}

	D3D11_INPUT_ELEMENT_DESC descs[]{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		//{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	hResult = device->CreateInputLayout(descs, 1, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), m_InputLayout.GetAddressOf());
	if (FAILED(hResult)) {
		MessageBoxA(0, "Failed CreateInputLayout()", "Fatal Error", MB_OK);
		PostQuitMessage(0);
	}
}

void CShaderDX11TestShader::CreatePS(ID3D11Device* device)
{
	ComPtr<ID3DBlob> pBlob = CompileHLSL(L"Shaders\\TestShader1.hlsl", "psTest", "ps_5_0");
	HRESULT hResult = device->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, m_PS.GetAddressOf());

	if (FAILED(hResult)) {
		MessageBoxA(0, "Failed CreatePixelShader()", "Fatal Error", MB_OK);
		PostQuitMessage(0);
	}
}

// ============================================================

CPhongLightShaderDX11::CPhongLightShaderDX11(ID3D11Device* device)
{
	CreateVSAndInputLayout(device);
	CreatePS(device);
}

void CPhongLightShaderDX11::ShaderReCompile(void* device)
{
	ID3D11Device* td = reinterpret_cast<ID3D11Device*>(device);

	CreateVSAndInputLayout(td);
	CreatePS(td);
}

void CPhongLightShaderDX11::CreateVSAndInputLayout(ID3D11Device* device)
{
	ComPtr<ID3DBlob> pBlob = CompileHLSL(L"Shaders\\PhongShader.hlsl", "vsmain", "vs_5_0");
	HRESULT hResult = device->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, m_VS.GetAddressOf());

	if (FAILED(hResult)) {
		MessageBoxA(0, "Failed CreateVertexShader()", "Fatal Error", MB_OK);
		PostQuitMessage(0);
	}

	D3D11_INPUT_ELEMENT_DESC descs[]{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	hResult = device->CreateInputLayout(descs, ARRAYSIZE(descs), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), m_InputLayout.GetAddressOf());
	if (FAILED(hResult)) {
		MessageBoxA(0, "Failed CreateInputLayout()", "Fatal Error", MB_OK);
		PostQuitMessage(0);
	}
}

void CPhongLightShaderDX11::CreatePS(ID3D11Device* device)
{
	ComPtr<ID3DBlob> pBlob = CompileHLSL(L"Shaders\\PhongShader.hlsl", "psmain", "ps_5_0");
	HRESULT hResult = device->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, m_PS.GetAddressOf());

	if (FAILED(hResult)) {
		MessageBoxA(0, "Failed CreatePixelShader()", "Fatal Error", MB_OK);
		PostQuitMessage(0);
	}
}

// =============================================================

CDeferredRenderingOnePathShaderDX11::CDeferredRenderingOnePathShaderDX11(ID3D11Device* device)
{
	CreateVSAndInputLayout(device);
	CreatePS(device);
}

void CDeferredRenderingOnePathShaderDX11::ShaderReCompile(void* device)
{
	ID3D11Device* td = reinterpret_cast<ID3D11Device*>(device);

	CreateVSAndInputLayout(td);
	CreatePS(td);
}

void CDeferredRenderingOnePathShaderDX11::CreateVSAndInputLayout(ID3D11Device* device)
{
	ComPtr<ID3DBlob> pBlob = CompileHLSL(L"Shaders\\DeferredRenderingShader.hlsl", "DeferredRenderOnePathVS", "vs_5_0");
	HRESULT hResult = device->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, m_VS.GetAddressOf());

	if (FAILED(hResult)) {
		MessageBoxA(0, "Failed CreateVertexShader()", "Fatal Error", MB_OK);
		PostQuitMessage(0);
	}

	D3D11_INPUT_ELEMENT_DESC descs[]{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	hResult = device->CreateInputLayout(descs, ARRAYSIZE(descs), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), m_InputLayout.GetAddressOf());
	if (FAILED(hResult)) {
		MessageBoxA(0, "Failed CreateInputLayout()", "Fatal Error", MB_OK);
		PostQuitMessage(0);
	}
}

void CDeferredRenderingOnePathShaderDX11::CreatePS(ID3D11Device* device)
{
	ComPtr<ID3DBlob> pBlob = CompileHLSL(L"Shaders\\DeferredRenderingShader.hlsl", "DeferredRenderOnePathPS", "ps_5_0");
	HRESULT hResult = device->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, m_PS.GetAddressOf());

	if (FAILED(hResult)) {
		MessageBoxA(0, "Failed CreatePixelShader()", "Fatal Error", MB_OK);
		PostQuitMessage(0);
	}
}

// Two Path =================================================================

CDeferredRenderingTwoPathShaderDX11::CDeferredRenderingTwoPathShaderDX11(ID3D11Device* device)
{
	CreateDepthStencilState(device);
	CreateVSAndInputLayout(device);
	CreatePS(device);
}

void CDeferredRenderingTwoPathShaderDX11::CreateDepthStencilState(ID3D11Device* device)
{
	D3D11_DEPTH_STENCIL_DESC desc{};
	desc.DepthEnable = FALSE;
	device->CreateDepthStencilState(&desc, m_DepthStencilState.GetAddressOf());
}

void CDeferredRenderingTwoPathShaderDX11::ShaderReCompile(void* device)
{
	ID3D11Device* td = reinterpret_cast<ID3D11Device*>(device);

	m_SamplerStates.clear();
	m_Samplers.clear();

	CreateVSAndInputLayout(td);
	CreatePS(td);
}

void CDeferredRenderingTwoPathShaderDX11::CreateVSAndInputLayout(ID3D11Device* device)
{
	ComPtr<ID3DBlob> pBlob = CompileHLSL(L"Shaders\\DeferredRenderingShader.hlsl", "DeferredRenderTwoPathVS", "vs_5_0");
	HRESULT hResult = device->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, m_VS.GetAddressOf());

	if (FAILED(hResult)) {
		MessageBoxA(0, "Failed CreateVertexShader()", "Fatal Error", MB_OK);
		PostQuitMessage(0);
	}
}

void CDeferredRenderingTwoPathShaderDX11::CreatePS(ID3D11Device* device)
{
	ComPtr<ID3DBlob> pBlob = CompileHLSL(L"Shaders\\DeferredRenderingShader.hlsl", "DeferredRenderTwoPathPS", "ps_5_0");
	HRESULT hResult = device->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, m_PS.GetAddressOf());

	if (FAILED(hResult)) {
		MessageBoxA(0, "Failed CreatePixelShader()", "Fatal Error", MB_OK);
		PostQuitMessage(0);
	}

	ComPtr<ID3D11SamplerState> sampler{};

	D3D11_SAMPLER_DESC desc{};
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	hResult = device->CreateSamplerState(&desc, sampler.GetAddressOf());
	if (FAILED(hResult)) {
		MessageBoxA(0, "Failed CreateSamplerState()", "Fatal Error", MB_OK);
		PostQuitMessage(0);
	}

	m_SamplerStates.push_back(sampler);
	m_Samplers.push_back(sampler.Get());
}