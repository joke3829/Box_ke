// ==================================================
// Shaders.h
// Base가 될 셰이더를 정리해둔 .h
// ==================================================
#pragma once
#include "stdafx.h"

class CShader {
public:
	// command: DX11 -> DeviceContext, DX12 -> CommandList
	virtual void SetShader(void* command) {}
	
	// Shader ReCompile in Runtime, 될지 의문
	virtual void ShaderReCompile(void* device) {};
protected:
	ComPtr<ID3DBlob> CompileHLSL(LPCWSTR pFileName, LPCSTR pEntryPoint, LPCSTR pTarget);
};

// DX11 =====================================================================================

class CShaderDX11 : public CShader {
	void SetShader(void* command);
	virtual void ShaderReCompile(void* device) {};
	// Render가 필요할지 고민
protected:
	// Guideline
	virtual void CreateBlendState(ID3D11Device* device) {};
	virtual void CreateDepthStencilState(ID3D11Device* device) {};
	virtual void CreateRasterizerState(ID3D11Device* device) {};

	virtual void CreateVSAndInputLayout(ID3D11Device* device) {};
	virtual void CreateHS(ID3D11Device* device) {};
	virtual void CreateDS(ID3D11Device* device) {};
	virtual void CreateGS(ID3D11Device* device) {};
	virtual void CreatePS(ID3D11Device* device) {};	// SamplerState는 여기서 생성해준다

	// 이 두 멤버는 상속해 만드는 셰이더의 생성자에서 초기화해준다.
	UINT									m_StencilRef{ 1 };
	UINT									m_SamplerSlot{};

	ComPtr<ID3D11InputLayout>				m_InputLayout{};		// require
	ComPtr<ID3D11DepthStencilState>			m_DepthStencilState{};	
	ComPtr<ID3D11BlendState>				m_BlendState{};
	ComPtr<ID3D11RasterizerState>			m_RasterizerState{};
	std::vector<ComPtr<ID3D11SamplerState>>	m_SamplerStates{};		// PS가 요구하면 정의
	std::vector<ID3D11SamplerState*>		m_Samplers{};			// 복사연산을 하지 않기위해 메모리 희생

	ComPtr<ID3D11VertexShader>				m_VS{};
	ComPtr<ID3D11HullShader>				m_HS{};
	ComPtr<ID3D11DomainShader>				m_DS{};
	ComPtr<ID3D11GeometryShader>			m_GS{};
	ComPtr<ID3D11PixelShader>				m_PS{};
	// Compute는 따로 사용하겠다.
};

class CComputeShaderDX11 : public CShader {
public:
	void SetShader(void* command);
	virtual void ShaderReCompile(void* device);
protected:
	virtual void CreateCS(ID3D11Device* device) {}
	ComPtr<ID3D11ComputeShader>				m_CS{};
};

// DX12 =======================================================================================