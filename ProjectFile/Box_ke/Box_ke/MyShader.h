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