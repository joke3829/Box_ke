#pragma once
#include "Shaders.h"

class CShaderDX11DrawTriangleShader : public CShaderDX11 {
public:
	CShaderDX11DrawTriangleShader(ID3D11Device* device);
	void ShaderReCompile(void* device);
protected:
	void CreateVSAndInputLayout(ID3D11Device* device);
	void CreatePS(ID3D11Device* device);
};


class CShaderDX11TestShader : public CShaderDX11 {
public:
	CShaderDX11TestShader(ID3D11Device* device);
	void ShaderReCompile(void* device);
protected:
	void CreateVSAndInputLayout(ID3D11Device* device);
	void CreatePS(ID3D11Device* device);
};

class CPhongLightShaderDX11 : public CShaderDX11 {
public:
	CPhongLightShaderDX11(ID3D11Device* device);
	void ShaderReCompile(void* device);
protected:
	void CreateVSAndInputLayout(ID3D11Device* device);
	void CreatePS(ID3D11Device* device);
};

class CDeferredRenderingOnePathShaderDX11 : public CShaderDX11 {
public:
	CDeferredRenderingOnePathShaderDX11(ID3D11Device* device);
	void ShaderReCompile(void* device);
protected:
	void CreateVSAndInputLayout(ID3D11Device* device);
	void CreatePS(ID3D11Device* device);
};

class CDeferredRenderingTwoPathShaderDX11 : public CShaderDX11 {
public:
	CDeferredRenderingTwoPathShaderDX11(ID3D11Device* device);
	void ShaderReCompile(void* device);
protected:
	void CreateDepthStencilState(ID3D11Device* device);
	void CreateVSAndInputLayout(ID3D11Device* device);
	void CreatePS(ID3D11Device* device);
};

class CResultRenderShaderDX11 : public CShaderDX11 {
public:
	CResultRenderShaderDX11(ID3D11Device* device);
	void ShaderReCompile(void* device);
protected:
	void CreateDepthStencilState(ID3D11Device* device);
	void CreateVSAndInputLayout(ID3D11Device* device);
	void CreatePS(ID3D11Device* device);
};

// ComputeShader

class CLuminanceComputeShaderDX11 : public CComputeShaderDX11 {
public:
	CLuminanceComputeShaderDX11(ID3D11Device* device);
protected:
	void CreateCS(ID3D11Device* device);
};