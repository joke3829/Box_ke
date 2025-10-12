// ========================================================
// Mesh.h
// 정점 정보나 인덱스 정보를 저장하는 헤더
// ========================================================

#pragma once
#include "stdafx.h"

class CMesh {
public:
	CMesh(float width, float height, bool BasedOnViewport = false);		// Plane
	CMesh(float width, float height, float depth, XMFLOAT3 center = {}, bool haveIndex = false);		// Box
	CMesh(float radius, UINT sampleCount = 8, XMFLOAT3 center = {});					// Sphere

	BoundingOrientedBox GetOBB();
	BoundingSphere GetBoundingSphere();

	virtual void Render(void* command) {}
protected:
	std::vector<XMFLOAT3>			m_Vertices{};
	std::vector<std::vector<UINT>>	m_Indices{};

	UINT							m_VertexStride{};
	UINT							m_VertexOffset{};

	// 상위 4비트 = OBB를 가짐, 하위 4비트 = BoundingSphere를 가짐
	BoundingOrientedBox				m_MeshOBB{};
	BoundingSphere					m_MeshBoundingSphere{};
	BYTE							m_HaveBoundingInfo{};	
};


// DX11
class CMeshDX11 : public CMesh {
public:
	CMeshDX11(void* device, float width, float height, void* command = nullptr, bool BasedOnViewport = false);
	CMeshDX11(void* device, float width, float height, float depth, void* command = nullptr, XMFLOAT3 center = {}, bool haveIndex = false);
	CMeshDX11(float radius, UINT sampleCount = 8, XMFLOAT3 center = {});

	virtual void Render(void* command);
protected:
	void InitVertexAndIndexBuffers(void* device, void* command = nullptr);

	ComPtr<ID3D11Buffer>				m_VertexBuffer{};
	std::vector<ComPtr<ID3D11Buffer>>	m_IndexBuffers{};

	D3D11_PRIMITIVE_TOPOLOGY			m_PT{};
};
