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

CPhongLightShader::CPhongLightShader(ID3D11Device* device)
{
	CreateVSAndInputLayout(device);
	CreatePS(device);
}

void CPhongLightShader::ShaderReCompile(void* device)
{
	ID3D11Device* td = reinterpret_cast<ID3D11Device*>(device);

	CreateVSAndInputLayout(td);
	CreatePS(td);
}

void CPhongLightShader::CreateVSAndInputLayout(ID3D11Device* device)
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

void CPhongLightShader::CreatePS(ID3D11Device* device)
{
	ComPtr<ID3DBlob> pBlob = CompileHLSL(L"Shaders\\PhongShader.hlsl", "psmain", "ps_5_0");
	HRESULT hResult = device->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, m_PS.GetAddressOf());

	if (FAILED(hResult)) {
		MessageBoxA(0, "Failed CreatePixelShader()", "Fatal Error", MB_OK);
		PostQuitMessage(0);
	}
}