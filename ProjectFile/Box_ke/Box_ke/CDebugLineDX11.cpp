#include "CDebugLineDX11.h"

void CDebugLineDX11::Initialize(void* device)
{
    ID3D11Device* dev = reinterpret_cast<ID3D11Device*>(device);

    D3D11_BUFFER_DESC desc{};
    desc.ByteWidth = sizeof(XMFLOAT3) * 2;          
    desc.Usage = D3D11_USAGE_DYNAMIC;              
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    dev->CreateBuffer(&desc, nullptr, m_VB.GetAddressOf());
}

void CDebugLineDX11::Render(void* command)
{
    ID3D11DeviceContext* context = reinterpret_cast<ID3D11DeviceContext*>(command);

 
    XMFLOAT3 v[2] = { m_P0, m_P1 };

    D3D11_MAPPED_SUBRESOURCE mapped{};
    context->Map(m_VB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    memcpy(mapped.pData, v, sizeof(v));
    context->Unmap(m_VB.Get(), 0);


    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    ID3D11Buffer* vb = m_VB.Get();
    context->IASetVertexBuffers(0, 1, &vb, &m_Stride, &m_Offset);

    context->Draw(2, 0);
}
