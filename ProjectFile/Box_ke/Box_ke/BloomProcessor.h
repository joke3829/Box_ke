#pragma once

#include "stdafx.h"
#include "MyShader.h"
#include "BlurProcessor.h"

class CBloomProcessor {
public:
	CBloomProcessor(UINT width, UINT height);

	virtual void ResizeBuffer(UINT width, UINT height, void* command) {}

	virtual void Process(void* command) {}
	virtual void Process(void* command, void* in_texture, void* out_texture = nullptr) {}
protected:
	UINT								m_clientWidth{};
	UINT								m_clientHeight;

	//std::shared_ptr<CShader>			m_TextureRenderShader{};
	std::shared_ptr<CShader>			m_TestShader{};
	std::shared_ptr<CShader>			m_LuminanceShader{};
	std::shared_ptr<CShader>			m_BleedingShader{};
};

class CBloomProcessorDX11 : public CBloomProcessor {
public:
	CBloomProcessorDX11(UINT width, UINT height, ID3D11Device* device);
	CBloomProcessorDX11(UINT width, UINT height, ID3D11Device* device, std::shared_ptr<CShader> textureRenderShader);

	void Process(void* command, void* in_texture, void* out_texture = nullptr);
protected:
	void CreateBuffers(ID3D11Device* device);
	void CreateBlurProcessor(ID3D11Device* device);

	std::shared_ptr<CGaussianBlurProcessor>		m_BlurProcessor6x6_1{};
	std::shared_ptr<CGaussianBlurProcessor>		m_BlurProcessor6x6_2{};
	std::shared_ptr<CGaussianBlurProcessor>		m_BlurProcessor4x4{};
	std::shared_ptr<CGaussianBlurProcessor>		m_BlurProcessorOrigin{};

	ComPtr<ID3D11Texture2D>					m_OriginTexture{};
	ComPtr<ID3D11ShaderResourceView>		m_OriginSRV{};
	ComPtr<ID3D11RenderTargetView>			m_OriginRTV{};

	ComPtr<ID3D11Texture2D>					m_4x4DownSampleTexture{};
	ComPtr<ID3D11ShaderResourceView>		m_4x4DownSampleSRV{};
	ComPtr<ID3D11RenderTargetView>			m_4x4DownSampleRTV{};

	ComPtr<ID3D11Texture2D>					m_6x6DownSampleTexture1{};
	ComPtr<ID3D11ShaderResourceView>		m_6x6DownSampleSRV1{};
	ComPtr<ID3D11RenderTargetView>			m_6x6DownSampleRTV1{};

	ComPtr<ID3D11Texture2D>					m_6x6DownSampleTexture2{};
	ComPtr<ID3D11ShaderResourceView>		m_6x6DownSampleSRV2{};
	ComPtr<ID3D11RenderTargetView>			m_6x6DownSampleRTV2{};

	ComPtr<ID3D11Texture2D>					m_BleedingResult1{};
	ComPtr<ID3D11ShaderResourceView>		m_BleedingSRV1{};
	ComPtr<ID3D11RenderTargetView>			m_BleedingRTV1{};

	ComPtr<ID3D11Texture2D>					m_BleedingResult2{};
	ComPtr<ID3D11ShaderResourceView>		m_BleedingSRV2{};
	ComPtr<ID3D11RenderTargetView>			m_BleedingRTV2{};

	ComPtr<ID3D11Texture2D>					m_FinalBleedingResult{};
	ComPtr<ID3D11RenderTargetView>			m_FinalBleedingRTV{};

	ComPtr<ID3D11Texture2D>					m_OriginSize2D{};
	ComPtr<ID3D11ShaderResourceView>		m_OriginSizeSRV{};
	ComPtr<ID3D11RenderTargetView>			m_OriginSizeRTV{};

	XMUINT3									m_4x4DispatchFactor{};
	XMUINT3									m_6x6DisaptchFactor1{};

	D3D11_VIEWPORT							m_ViewPort[4]{};
};