// ==================================================
// Shaders.h
// Base�� �� ���̴��� �����ص� .h
// ==================================================
#pragma once
#include "stdafx.h"

class CShader {
public:
	// command: DX11 -> DeviceContext, DX12 -> CommandList
	virtual void SetShader(void* command) {}
	
	// Shader ReCompile in Runtime, ���� �ǹ�
	virtual void ShaderReCompile(void* device) {};
protected:
	ComPtr<ID3DBlob> CompileHLSL(LPCWSTR pFileName, LPCSTR pEntryPoint, LPCSTR pTarget);
};

// DX11 =====================================================================================

class CShaderDX11 : public CShader {
	void SetShader(void* command);
	virtual void ShaderReCompile(void* device) {};
	// Render�� �ʿ����� ���
protected:
	// Guideline
	virtual void CreateBlendState(ID3D11Device* device) {};
	virtual void CreateDepthStencilState(ID3D11Device* device) {};
	virtual void CreateRasterizerState(ID3D11Device* device) {};

	virtual void CreateVSAndInputLayout(ID3D11Device* device) {};
	virtual void CreateHS(ID3D11Device* device) {};
	virtual void CreateDS(ID3D11Device* device) {};
	virtual void CreateGS(ID3D11Device* device) {};
	virtual void CreatePS(ID3D11Device* device) {};	// SamplerState�� ���⼭ �������ش�

	// �� �� ����� ����� ����� ���̴��� �����ڿ��� �ʱ�ȭ���ش�.
	UINT									m_StencilRef{ 1 };
	UINT									m_SamplerSlot{};

	ComPtr<ID3D11InputLayout>				m_InputLayout{};		// require
	ComPtr<ID3D11DepthStencilState>			m_DepthStencilState{};	
	ComPtr<ID3D11BlendState>				m_BlendState{};
	ComPtr<ID3D11RasterizerState>			m_RasterizerState{};
	std::vector<ComPtr<ID3D11SamplerState>>	m_SamplerStates{};		// PS�� �䱸�ϸ� ����
	std::vector<ID3D11SamplerState*>		m_Samplers{};			// ���翬���� ���� �ʱ����� �޸� ���

	ComPtr<ID3D11VertexShader>				m_VS{};
	ComPtr<ID3D11HullShader>				m_HS{};
	ComPtr<ID3D11DomainShader>				m_DS{};
	ComPtr<ID3D11GeometryShader>			m_GS{};
	ComPtr<ID3D11PixelShader>				m_PS{};
	// Compute�� ���� ����ϰڴ�.
};

// DX12 =======================================================================================