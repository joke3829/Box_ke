#pragma once
#include "stdafx.h"
#include "Mesh.h"
#include "Material.h"
#include "Animation.h"

class CDebugLineDX11
{
public:
    void Initialize(void* device);
    void SetPoints(const XMFLOAT3& a, const XMFLOAT3& b) { m_P0 = a; m_P1 = b; }
    void Render(void* command);

private:
    XMFLOAT3 m_P0{}, m_P1{};
    ComPtr<ID3D11Buffer> m_VB{};
    UINT m_Stride = sizeof(XMFLOAT3);
    UINT m_Offset = 0;
};
