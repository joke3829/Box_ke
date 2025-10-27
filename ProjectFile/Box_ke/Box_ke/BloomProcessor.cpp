#include "BloomProcessor.h"

CBloomProcessor::CBloomProcessor(UINT width, UINT height)
	: m_clientWidth{ width }, m_clientHeight{ height }
{

}


// ================================================================

CBloomProcessorDX11::CBloomProcessorDX11(UINT width, UINT height, ID3D11Device* device)
	: CBloomProcessor(width, height)
{
	CreateBuffers(device);
	CreateBlurProcessor(device);
	m_LuminanceShader = std::make_shared<CLuminanceShaderDX11>(device);
	m_TextureRenderShader = std::make_shared<CFullScreenTextrueRenderShaderDX11>(device);
	m_BleedingShader = std::make_shared<CTextureBleedingShaderDX11>(device);
}

CBloomProcessorDX11::CBloomProcessorDX11(UINT width, UINT height, ID3D11Device* device, std::shared_ptr<CShader> textureRenderShader)
	: CBloomProcessor(width, height)
{
	CreateBuffers(device);
	CreateBlurProcessor(device);
	m_LuminanceShader = std::make_shared<CLuminanceShaderDX11>(device);
	m_TextureRenderShader = textureRenderShader;
	m_BleedingShader = std::make_shared<CTextureBleedingShaderDX11>(device);
}

void CBloomProcessorDX11::CreateBuffers(ID3D11Device* device)
{
	XMUINT2 size4x4 = { m_clientWidth / 4, m_clientHeight / 4 };
	XMUINT2 size6x6_1 = { size4x4.x / 6, size4x4.y / 6 };
	XMUINT2 size6x6_2 = { size6x6_1.x / 6, size6x6_1.y / 6 };
	
	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = m_clientWidth;
	desc.Height = m_clientHeight;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	device->CreateTexture2D(&desc, nullptr, m_OriginTexture.GetAddressOf());
	device->CreateShaderResourceView(m_OriginTexture.Get(), nullptr, m_OriginSRV.GetAddressOf());
	device->CreateRenderTargetView(m_OriginTexture.Get(), nullptr, m_OriginRTV.GetAddressOf());
	
	device->CreateTexture2D(&desc, nullptr, m_FinalBleedingResult.GetAddressOf());
	device->CreateRenderTargetView(m_FinalBleedingResult.Get(), nullptr, m_FinalBleedingRTV.GetAddressOf());

	desc.Width = size4x4.x;
	desc.Height = size4x4.y;
	device->CreateTexture2D(&desc, nullptr, m_4x4DownSampleTexture.GetAddressOf());
	device->CreateShaderResourceView(m_4x4DownSampleTexture.Get(), nullptr, m_4x4DownSampleSRV.GetAddressOf());
	device->CreateRenderTargetView(m_4x4DownSampleTexture.Get(), nullptr, m_4x4DownSampleRTV.GetAddressOf());

	device->CreateTexture2D(&desc, nullptr, m_BleedingResult2.GetAddressOf());
	device->CreateShaderResourceView(m_BleedingResult2.Get(), nullptr, m_BleedingSRV2.GetAddressOf());
	device->CreateRenderTargetView(m_BleedingResult2.Get(), nullptr, m_BleedingRTV2.GetAddressOf());

	desc.Width = size6x6_1.x;
	desc.Height = size6x6_1.y;
	device->CreateTexture2D(&desc, nullptr, m_6x6DownSampleTexture1.GetAddressOf());
	device->CreateShaderResourceView(m_6x6DownSampleTexture1.Get(), nullptr, m_6x6DownSampleSRV1.GetAddressOf());
	device->CreateRenderTargetView(m_6x6DownSampleTexture1.Get(), nullptr, m_6x6DownSampleRTV1.GetAddressOf());

	device->CreateTexture2D(&desc, nullptr, m_BleedingResult1.GetAddressOf());
	device->CreateShaderResourceView(m_BleedingResult1.Get(), nullptr, m_BleedingSRV1.GetAddressOf());
	device->CreateRenderTargetView(m_BleedingResult1.Get(), nullptr, m_BleedingRTV1.GetAddressOf());

	desc.Width = size6x6_2.x;
	desc.Height = size6x6_2.y;
	device->CreateTexture2D(&desc, nullptr, m_6x6DownSampleTexture2.GetAddressOf());
	device->CreateShaderResourceView(m_6x6DownSampleTexture2.Get(), nullptr, m_6x6DownSampleSRV2.GetAddressOf());
	device->CreateRenderTargetView(m_6x6DownSampleTexture2.Get(), nullptr, m_6x6DownSampleRTV2.GetAddressOf());

	m_4x4DispatchFactor = { size4x4.x / 32 + 1, size4x4.y / 32 + 1 , 1};
	m_ViewPort[0] = { 0.f, 0.f, static_cast<float>(m_clientWidth), static_cast<float>(m_clientHeight), 0.f, 1.f };
	m_ViewPort[1] = { 0.f, 0.f, static_cast<float>(size4x4.x), static_cast<float>(size4x4.y), 0.f, 1.f };	// 4x4
	m_ViewPort[2] = { 0.f, 0.f, static_cast<float>(size6x6_1.x), static_cast<float>(size6x6_1.y), 0.f, 1.f };	// 6x6_1
	m_ViewPort[3] = { 0.f, 0.f, static_cast<float>(size6x6_2.x), static_cast<float>(size6x6_2.y), 0.f, 1.f };	// 6x6_2
}

void CBloomProcessorDX11::CreateBlurProcessor(ID3D11Device* device)
{
	std::shared_ptr<CShader> ver = std::make_shared<CVerticalGaussianBlurShaderDX11>(device);
	std::shared_ptr<CShader> hor = std::make_shared<CHorizonGaussianBlurShaderDX11>(device);

	XMUINT2 size4x4 = { m_clientWidth / 4, m_clientHeight / 4 };
	XMUINT2 size6x6_1 = { size4x4.x / 6, size4x4.y / 6 };
	XMUINT2 size6x6_2 = { size6x6_1.x / 6, size6x6_1.y / 6 };

	m_BlurProcessor6x6_1 = std::make_shared<CGaussianBlurProcessorDX11>(size6x6_2.x, size6x6_2.y, device, ver, hor);
	m_BlurProcessor6x6_2 = std::make_shared<CGaussianBlurProcessorDX11>(size6x6_1.x, size6x6_1.y, device, ver, hor);
	m_BlurProcessor4x4 = std::make_shared<CGaussianBlurProcessorDX11>(size4x4.x, size4x4.y, device, ver, hor);
}

void CBloomProcessorDX11::Process(void* command, void* in_texture, void* out_texture)
{
	ID3D11DeviceContext* context = reinterpret_cast<ID3D11DeviceContext*>(command);
	ID3D11Texture2D* itxt = reinterpret_cast<ID3D11Texture2D*>(in_texture);
	ID3D11Texture2D* otxt;
	if (out_texture)
		otxt = reinterpret_cast<ID3D11Texture2D*>(out_texture);
	else
		otxt = reinterpret_cast<ID3D11Texture2D*>(in_texture);

	context->CopyResource(m_OriginTexture.Get(), itxt);
	// 4x4Down Sampling
	m_LuminanceShader->SetShader(context);
	context->OMSetRenderTargets(1, m_4x4DownSampleRTV.GetAddressOf(), nullptr);
	context->RSSetViewports(1, &m_ViewPort[1]);
	context->PSSetShaderResources(0, 1, m_OriginSRV.GetAddressOf());

	context->DrawInstanced(6, 1, 0, 0);

	// 6x6_1 Down Sampling
	context->OMSetRenderTargets(1, m_6x6DownSampleRTV1.GetAddressOf(), nullptr);
	context->RSSetViewports(1, &m_ViewPort[2]);
	context->PSSetShaderResources(0, 1, m_4x4DownSampleSRV.GetAddressOf());

	context->DrawInstanced(6, 1, 0, 0);

	// 6x6_2 Down Sampling
	context->OMSetRenderTargets(1, m_6x6DownSampleRTV2.GetAddressOf(), nullptr);
	context->RSSetViewports(1, &m_ViewPort[3]);
	context->PSSetShaderResources(0, 1, m_6x6DownSampleSRV1.GetAddressOf());

	context->DrawInstanced(6, 1, 0, 0);

	// Blur(6x6_2)
	m_BlurProcessor6x6_1->Process(1, context, m_6x6DownSampleTexture2.Get());

	// Blur(6x6_1 + Blur(6x6_2))
	m_BleedingShader->SetShader(context);
	context->OMSetRenderTargets(1, m_BleedingRTV1.GetAddressOf(), nullptr);
	context->RSSetViewports(1, &m_ViewPort[2]);
	ID3D11ShaderResourceView* temp[] = { m_6x6DownSampleSRV1.Get(), m_6x6DownSampleSRV2.Get() };
	context->PSSetShaderResources(0, 2, temp);
	context->DrawInstanced(6, 1, 0, 0);
	m_BlurProcessor6x6_2->Process(1, context, m_BleedingResult1.Get());

	// Blur(4x4 + Blur(6x6_1 + Blur(6x6_2)))
	context->OMSetRenderTargets(1, m_BleedingRTV2.GetAddressOf(), nullptr);
	context->RSSetViewports(1, &m_ViewPort[1]);
	temp[0] = m_4x4DownSampleSRV.Get(), temp[1] = m_BleedingSRV1.Get();
	context->PSSetShaderResources(0, 2, temp);
	context->DrawInstanced(6, 1, 0, 0);
	m_BlurProcessor6x6_2->Process(1, context, m_BleedingResult2.Get());

	// 원복
	//m_TextureRenderShader->SetShader(context);
	context->OMSetRenderTargets(1, m_FinalBleedingRTV.GetAddressOf(), nullptr);
	context->RSSetViewports(1, &m_ViewPort[0]);
	temp[0] = m_OriginSRV.Get(), temp[1] = m_BleedingSRV2.Get();
	context->PSSetShaderResources(0, 2, temp);
	context->DrawInstanced(6, 1, 0, 0);

	context->CopyResource(otxt, m_FinalBleedingResult.Get());
}