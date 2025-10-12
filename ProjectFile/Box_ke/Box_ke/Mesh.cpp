#include "Mesh.h"

// BasedOnViewport = true, 평면의 좌측위를 중심으로 평면을 정의
CMesh::CMesh(float width, float height, bool BasedOnViewport)
{
	if (BasedOnViewport) {
		m_Vertices.emplace_back(0.f, -height, 0.f);
		m_Vertices.emplace_back(width, -height, 0.f);
		m_Vertices.emplace_back(width, 0.f, 0.f);
		m_Vertices.emplace_back(0.f, 0.f, 0.f);
		m_MeshOBB = BoundingOrientedBox(
			XMFLOAT3(width / 2.f, -height / 2.f, 0.f),
			XMFLOAT3(width / 2.f, height / 2.f, 0.001f),
			XMFLOAT4(0.f, 0.f, 0.f, 1.f)
		);
	}
	else {
		float halfW = width / 2;
		float halfH = height / 2;
		m_Vertices.emplace_back(-halfW, -halfH, 0.f);
		m_Vertices.emplace_back(halfW, -halfH , 0.f);
		m_Vertices.emplace_back(halfW, halfH, 0.f);
		m_Vertices.emplace_back(-halfW, halfH, 0.f);

		m_MeshOBB = BoundingOrientedBox(
			XMFLOAT3(0.f, 0.f, 0.f),
			XMFLOAT3(halfW, halfH, 0.001f),
			XMFLOAT4(0.f, 0.f, 0.f, 1.f)
		);
	}
	m_HaveBoundingInfo = 0xf0;
	std::vector<UINT> index(6, 0);
	index[0] = 0; index[1] = 3; index[2] = 1;
	index[3] = 3; index[4] = 2; index[5] = 1;

	m_Indices.push_back(index);

	m_VertexStride = sizeof(XMFLOAT3);
}

CMesh::CMesh(float width, float height, float depth, XMFLOAT3 center, bool haveIndex)
{
	float halfW = width / 2;
	float halfH = height / 2;
	float halfD = depth / 2;
	if (haveIndex) {
		m_Vertices.emplace_back(center.x - halfW, center.y - halfH, center.z - halfD);
		m_Vertices.emplace_back(center.x + halfW, center.y - halfH, center.z - halfD);
		m_Vertices.emplace_back(center.x + halfW, center.y - halfH, center.z + halfD);
		m_Vertices.emplace_back(center.x - halfW, center.y - halfH, center.z + halfD);
		m_Vertices.emplace_back(center.x - halfW, center.y + halfH, center.z - halfD);
		m_Vertices.emplace_back(center.x + halfW, center.y + halfH, center.z - halfD);
		m_Vertices.emplace_back(center.x + halfW, center.y + halfH, center.z + halfD);
		m_Vertices.emplace_back(center.x - halfW, center.y + halfH, center.z + halfD);

		std::vector<UINT> index(36, 0);
		index[0] = 0; index[1] = 1; index[2] = 2;
		index[3] = 0; index[4] = 2; index[5] = 3;

		index[6] = 0; index[7] = 4; index[8] = 5;
		index[9] = 0; index[10] = 5; index[11] = 1;

		index[12] = 1; index[13] = 5; index[14] = 6;
		index[15] = 1; index[16] = 6; index[17] = 2;

		index[18] = 2; index[19] = 6; index[20] = 7;
		index[21] = 2; index[22] = 7; index[23] = 3;

		index[24] = 3; index[25] = 7; index[26] = 4;
		index[27] = 3; index[28] = 4; index[29] = 0;

		index[30] = 4; index[31] = 7; index[32] = 6;
		index[33] = 4; index[34] = 6; index[35] = 5;

		m_Indices.push_back(index);
	}
	else {
		m_Vertices.emplace_back(center.x - halfW, center.y - halfH, center.z - halfD);//0
		m_Vertices.emplace_back(center.x + halfW, center.y - halfH, center.z - halfD);//1
		m_Vertices.emplace_back(center.x + halfW, center.y - halfH, center.z + halfD);//2
		m_Vertices.emplace_back(center.x - halfW, center.y - halfH, center.z - halfD);//0
		m_Vertices.emplace_back(center.x + halfW, center.y - halfH, center.z + halfD);//2
		m_Vertices.emplace_back(center.x - halfW, center.y - halfH, center.z + halfD);//3

		m_Vertices.emplace_back(center.x - halfW, center.y - halfH, center.z - halfD);//0
		m_Vertices.emplace_back(center.x - halfW, center.y + halfH, center.z - halfD);//4
		m_Vertices.emplace_back(center.x + halfW, center.y + halfH, center.z - halfD);//5
		m_Vertices.emplace_back(center.x - halfW, center.y - halfH, center.z - halfD);//0
		m_Vertices.emplace_back(center.x + halfW, center.y + halfH, center.z - halfD);//5
		m_Vertices.emplace_back(center.x + halfW, center.y - halfH, center.z - halfD);//1

		m_Vertices.emplace_back(center.x + halfW, center.y - halfH, center.z - halfD);//1
		m_Vertices.emplace_back(center.x + halfW, center.y + halfH, center.z - halfD);//5
		m_Vertices.emplace_back(center.x + halfW, center.y + halfH, center.z + halfD);//6
		m_Vertices.emplace_back(center.x + halfW, center.y - halfH, center.z - halfD);//1
		m_Vertices.emplace_back(center.x + halfW, center.y + halfH, center.z + halfD);//6
		m_Vertices.emplace_back(center.x + halfW, center.y - halfH, center.z + halfD);//2

		m_Vertices.emplace_back(center.x + halfW, center.y - halfH, center.z + halfD);//2
		m_Vertices.emplace_back(center.x + halfW, center.y + halfH, center.z + halfD);//6
		m_Vertices.emplace_back(center.x - halfW, center.y + halfH, center.z + halfD);//7
		m_Vertices.emplace_back(center.x + halfW, center.y - halfH, center.z + halfD);//2
		m_Vertices.emplace_back(center.x - halfW, center.y + halfH, center.z + halfD);//7
		m_Vertices.emplace_back(center.x - halfW, center.y - halfH, center.z + halfD);//3

		m_Vertices.emplace_back(center.x - halfW, center.y - halfH, center.z + halfD);//3
		m_Vertices.emplace_back(center.x - halfW, center.y + halfH, center.z + halfD);//7
		m_Vertices.emplace_back(center.x - halfW, center.y + halfH, center.z - halfD);//4
		m_Vertices.emplace_back(center.x - halfW, center.y - halfH, center.z + halfD);//3
		m_Vertices.emplace_back(center.x - halfW, center.y + halfH, center.z - halfD);//4
		m_Vertices.emplace_back(center.x - halfW, center.y - halfH, center.z - halfD);//0

		m_Vertices.emplace_back(center.x - halfW, center.y + halfH, center.z - halfD);//4
		m_Vertices.emplace_back(center.x - halfW, center.y + halfH, center.z + halfD);//7
		m_Vertices.emplace_back(center.x + halfW, center.y + halfH, center.z + halfD);//6
		m_Vertices.emplace_back(center.x - halfW, center.y + halfH, center.z - halfD);//4
		m_Vertices.emplace_back(center.x + halfW, center.y + halfH, center.z + halfD);//6
		m_Vertices.emplace_back(center.x + halfW, center.y + halfH, center.z - halfD);//5
	}

	m_HaveBoundingInfo = 0xf0;
	m_MeshOBB = BoundingOrientedBox(XMFLOAT3(), XMFLOAT3(halfW, halfH, halfD), XMFLOAT4(0.f, 0.f, 0.f, 1.f));

	m_VertexStride = sizeof(XMFLOAT3);
}

CMesh::CMesh(float radius, UINT sampleCount, XMFLOAT3 center)
{

}

BoundingOrientedBox CMesh::GetOBB()
{
	return m_MeshOBB;
}

BoundingSphere CMesh::GetBoundingSphere()
{
	return m_MeshBoundingSphere;
}

// DX11===========================================================================

// device는 null이 될 수 없다
CMeshDX11::CMeshDX11(void* device, float width, float height, void* command, bool BasedOnViewport)
	: CMesh(width, height, BasedOnViewport)
{
	InitVertexAndIndexBuffers(device, command);
	m_PT = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

CMeshDX11::CMeshDX11(void* device, float width, float height, float depth, void* command, XMFLOAT3 center, bool haveIndex)
	: CMesh(width, height, depth, center, haveIndex)
{
	InitVertexAndIndexBuffers(device, command);
	m_PT = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

// 사용 금지
CMeshDX11::CMeshDX11(float radius, UINT sampleCount, XMFLOAT3 center)
	: CMesh(radius, sampleCount, center)
{

}

void CMeshDX11::InitVertexAndIndexBuffers(void* device, void* command)
{
	ID3D11Device* dev = reinterpret_cast<ID3D11Device*>(device);

	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = m_VertexStride * m_Vertices.size();
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = m_Vertices.data();
	dev->CreateBuffer(&desc, &data, m_VertexBuffer.GetAddressOf());


	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	for (int i = 0; i < m_Indices.size(); ++i) {
		desc.ByteWidth = sizeof(UINT) * m_Indices[i].size();
		data.pSysMem = m_Indices[i].data();
		ComPtr<ID3D11Buffer> tempBuffer;
		dev->CreateBuffer(&desc, &data, tempBuffer.GetAddressOf());
		m_IndexBuffers.push_back(tempBuffer);
	}
}

void CMeshDX11::Render(void* command)
{
	ID3D11DeviceContext* context = reinterpret_cast<ID3D11DeviceContext*>(command);
	context->IASetPrimitiveTopology(m_PT);
	context->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &m_VertexStride, &m_VertexOffset);
	if (m_IndexBuffers.size() != 0) {
		for (int i = 0; i < m_IndexBuffers.size(); ++i) {
			context->IASetIndexBuffer(m_IndexBuffers[i].Get(), DXGI_FORMAT_R32_UINT, 0);
			context->DrawIndexed(m_Indices[i].size(), 0, 0);
		}
	}
	else {
		context->DrawInstanced(m_Vertices.size() * 3, 1, 0, 0);
	}
}