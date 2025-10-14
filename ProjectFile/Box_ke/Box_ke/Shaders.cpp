#include "Shaders.h"

// Base============================================================

ComPtr<ID3DBlob> CShader::CompileHLSL(LPCWSTR pFileName, LPCSTR pEntryPoint, LPCSTR pTarget)
{
	ComPtr<ID3DBlob> pBlob{};
	ComPtr<ID3DBlob> pErrorBlob{};

	HRESULT hResult = D3DCompileFromFile(pFileName, nullptr, nullptr, pEntryPoint, pTarget, 0, 0, pBlob.GetAddressOf(), pErrorBlob.GetAddressOf());
	if (FAILED(hResult)) {
		const char* errorstr = static_cast<const char*>(pErrorBlob->GetBufferPointer());
		MessageBoxA(0, errorstr, "Shader Compile Error", MB_OK);
		PostQuitMessage(0);
	}
	
	return pBlob;
}

// ===============================================================



// DX11===========================================================

void CShaderDX11::SetShader(void* command)
{
	ID3D11DeviceContext* context = reinterpret_cast<ID3D11DeviceContext*>(command);

	context->IASetInputLayout(m_InputLayout.Get());

	if (m_DepthStencilState) context->OMSetDepthStencilState(m_DepthStencilState.Get(), m_StencilRef);
	else context->OMSetDepthStencilState(nullptr, m_StencilRef);

	if (m_BlendState) context->OMSetBlendState(m_BlendState.Get(), nullptr, 0xffffffff); // 필요 시 수정
	else context->OMSetBlendState(nullptr, nullptr, 0xffffffff);

	if (m_RasterizerState) context->RSSetState(m_RasterizerState.Get());
	else context->RSSetState(nullptr);

	context->VSSetShader(m_VS.Get(), nullptr, 0);		// 필수(아닐수도 있음)

	if (m_HS) {
		context->HSSetShader(m_HS.Get(), nullptr, 0);
		context->DSSetShader(m_DS.Get(), nullptr, 0);
	}
	else {
		context->HSSetShader(nullptr, nullptr, 0);
		context->DSSetShader(nullptr, nullptr, 0);
	}

	if (m_GS) context->GSSetShader(m_GS.Get(), nullptr, 0);
	else context->GSSetShader(nullptr, nullptr, 0);

	if (m_PS) {
		context->PSSetShader(m_PS.Get(), nullptr, 0);
		if (m_Samplers.size() > 0)
			context->PSSetSamplers(m_SamplerSlot, m_Samplers.size(), m_Samplers.data());
	}
	else
		context->PSSetShader(nullptr, nullptr, 0);
}

// ===============================================================



// DX12===========================================================

// ===============================================================