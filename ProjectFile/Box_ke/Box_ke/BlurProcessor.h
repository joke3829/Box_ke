#pragma once

#include "stdafx.h"
#include "MyShader.h"

class CGaussianBlurProcessor {
public:
	CGaussianBlurProcessor(UINT width, UINT height);

	virtual void ResizeBuffer(UINT width, UINT height) {}
	virtual void Process(UINT dispatchCount, void* command, void* in_texture, void* out_texture = nullptr) {}
protected:
	UINT							m_Width{};
	UINT							m_Height{};

	XMUINT2							m_DispatchFactor{};		// x: verticla x factor, y: horizon y factor

	std::shared_ptr<CShader>		m_VerticalShader{};
	std::shared_ptr<CShader>		m_HorizonShader{};
};

class CGaussianBlurProcessorDX11 : public CGaussianBlurProcessor {
public:
	CGaussianBlurProcessorDX11(UINT width, UINT height, ID3D11Device* device);
	CGaussianBlurProcessorDX11(UINT width, UINT height, ID3D11Device* device,
		std::shared_ptr<CShader> verShader, std::shared_ptr<CShader> horShader);

	void Process(UINT dispatchCount, void* command, void* in_texture, void* out_texture = nullptr);
protected:
	void CreateBuffer(ID3D11Device* device);

	ComPtr<ID3D11Texture2D>				m_CSInput{};
	ComPtr<ID3D11ShaderResourceView>	m_CSInputSRV{};

	ComPtr<ID3D11Texture2D>				m_CSOutput{};
	ComPtr<ID3D11UnorderedAccessView>	m_CSOutputUAV{};
};