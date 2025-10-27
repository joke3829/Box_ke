#include "BlurProcessor.h"

CGaussianBlurProcessor::CGaussianBlurProcessor(UINT width, UINT height)
	: m_Width{ width }, m_Height{ height }
{
	m_DispatchFactor = { width / 64 + 1, height / 64 + 1 };
}

CGaussianBlurProcessorDX11::CGaussianBlurProcessorDX11(UINT width, UINT height, ID3D11Device* device)
	: CGaussianBlurProcessor(width, height)
{
	// Shader Create
	m_VerticalShader = std::make_shared<CVerticalGaussianBlurShaderDX11>(device);
	m_HorizonShader = std::make_shared<CHorizonGaussianBlurShaderDX11>(device);
	CreateBuffer(device);
}

CGaussianBlurProcessorDX11::CGaussianBlurProcessorDX11(UINT width, UINT height, ID3D11Device* device,
	std::shared_ptr<CShader> verShader, std::shared_ptr<CShader> horShader)
	: CGaussianBlurProcessor(width, height)
{
	m_VerticalShader = verShader; m_HorizonShader = horShader;
	CreateBuffer(device);
}

void CGaussianBlurProcessorDX11::CreateBuffer(ID3D11Device* device)
{
	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = m_Width;
	desc.Height = m_Height;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	device->CreateTexture2D(&desc, nullptr, m_CSInput.GetAddressOf());
	device->CreateShaderResourceView(m_CSInput.Get(), nullptr, m_CSInputSRV.GetAddressOf());

	device->CreateTexture2D(&desc, nullptr, m_CSOutput.GetAddressOf());
	device->CreateUnorderedAccessView(m_CSOutput.Get(), nullptr, m_CSOutputUAV.GetAddressOf());
}

void CGaussianBlurProcessorDX11::Process(UINT dispatchCount, void* command, void* in_texture, void* out_texture)
{
	ID3D11DeviceContext* context = reinterpret_cast<ID3D11DeviceContext*>(command);
	ID3D11Texture2D* itxt = reinterpret_cast<ID3D11Texture2D*>(in_texture);
	ID3D11Texture2D* otxt;
	if (out_texture)
		otxt = reinterpret_cast<ID3D11Texture2D*>(out_texture);
	else
		otxt = reinterpret_cast<ID3D11Texture2D*>(in_texture);

	context->CopyResource(m_CSInput.Get(), itxt);
	context->CSSetShaderResources(0, 1, m_CSInputSRV.GetAddressOf());
	context->CSSetUnorderedAccessViews(0, 1, m_CSOutputUAV.GetAddressOf(), nullptr);

	for (UINT i = 0; i < dispatchCount; ++i) {
		m_HorizonShader->SetShader(context);
		context->Dispatch(m_DispatchFactor.x, m_Height, 1);
		context->CopyResource(m_CSInput.Get(), m_CSOutput.Get());

		m_VerticalShader->SetShader(context);
		context->Dispatch(m_Width, m_DispatchFactor.y, 1);
		context->CopyResource(m_CSInput.Get(), m_CSOutput.Get());
	}

	context->CopyResource(otxt, m_CSOutput.Get());
}