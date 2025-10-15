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
	if (sampleCount < 8) sampleCount = 8;

	m_Vertices.emplace_back(center.x, center.y + radius, center.z);

	float vertical = 180.f / (sampleCount + 1);
	float horizon = 360.f / sampleCount;

	for (UINT i = 0; i < sampleCount; ++i) {
		float ver = 90.0f - (vertical * static_cast<float>(i + 1));
		float radver = XMConvertToRadians(ver);
		float horRadius = radius * cosf(radver);
		for (UINT j = 0; j < sampleCount; ++j) {
			float hor = horizon * j;
			float radhor = XMConvertToRadians(hor);
			m_Vertices.emplace_back(center.x + (horRadius * cosf(radhor)), center.y + (radius * sinf(radver)), center.z + (horRadius * sinf(radhor)));
		}
	}
	m_Vertices.emplace_back(center.x, center.y - radius, center.z);

	std::vector<UINT> index;
	for (UINT i = 0; i < sampleCount; ++i) {
		index.emplace_back(0);
		index.emplace_back(i + 2);
		index.emplace_back(i + 1);
	}
	index[index.size() - 2] = 1;

	for (UINT i = 0; i < sampleCount - 1; ++i) {
		UINT line = i * sampleCount;
		UINT nextline = (i + 1) * sampleCount;
		for (UINT j = 1; j <= sampleCount; ++j) {
			if (j != sampleCount) {
				index.emplace_back(j + line);
				index.emplace_back(j + 1 + line);
				index.emplace_back(j + nextline);

				index.emplace_back(j + nextline);
				index.emplace_back(j + 1 + line);
				index.emplace_back(j + 1 + nextline);
			}
			else {
				index.emplace_back(j + line);
				index.emplace_back(1 + line);
				index.emplace_back(j + nextline);

				index.emplace_back(j + nextline);
				index.emplace_back(1 + line);
				index.emplace_back(1 + nextline);
			}
		}
	}

	UINT lastIndex = 1 + sampleCount * sampleCount;
	for (UINT i = 0; i < sampleCount; ++i) {
		index.emplace_back(i + lastIndex - sampleCount);
		index.emplace_back(i + lastIndex - sampleCount + 1);
		index.emplace_back(lastIndex);
	}
	index[index.size() - 2] = lastIndex - sampleCount;

	m_Indices.push_back(index);

	m_HaveBoundingInfo = 0x0f;
	m_MeshBoundingSphere = BoundingSphere(center, radius);

	m_VertexStride = sizeof(XMFLOAT3);
}

BoundingOrientedBox CMesh::GetOBB()
{
	return m_MeshOBB;
}

BoundingSphere CMesh::GetBoundingSphere()
{
	return m_MeshBoundingSphere;
}

CNormalMesh::CNormalMesh(float width, float height, bool BasedOnViewport)
	: CMesh(width, height, BasedOnViewport)
{
	m_Normals.reserve(4);
	for (int i = 0; i < 4; ++i)
		m_Normals.emplace_back(0.f, 0.f, -1.f);

	m_NormalStride = sizeof(XMFLOAT3);
}

CNormalMesh::CNormalMesh(float width, float height, float depth, XMFLOAT3 center, bool haveIndex)
	: CMesh(width, height, depth, center, haveIndex)
{
	if (haveIndex) {	// 정육면체가 아니면 부정확함, 사용 권장 x
		m_Normals.assign(8, {});
		XMVECTOR nor = XMVectorSet(-1.f, -1.f, -1.f, 0.f);
		XMStoreFloat3(&m_Normals[0], XMVector3Normalize(nor));
		nor = XMVectorSet(1.f, -1.f, -1.f, 0.f);
		XMStoreFloat3(&m_Normals[1], XMVector3Normalize(nor));
		nor = XMVectorSet(1.f, -1.f, 1.f, 0.f);
		XMStoreFloat3(&m_Normals[2], XMVector3Normalize(nor));
		nor = XMVectorSet(-1.f, -1.f, 1.f, 0.f);
		XMStoreFloat3(&m_Normals[3], XMVector3Normalize(nor));
		nor = XMVectorSet(-1.f, 1.f, -1.f, 0.f);
		XMStoreFloat3(&m_Normals[4], XMVector3Normalize(nor));
		nor = XMVectorSet(1.f, 1.f, -1.f, 0.f);
		XMStoreFloat3(&m_Normals[5], XMVector3Normalize(nor));
		nor = XMVectorSet(1.f, 1.f, 1.f, 0.f);
		XMStoreFloat3(&m_Normals[6], XMVector3Normalize(nor));
		nor = XMVectorSet(-1.f, 1.f, 1.f, 0.f);
		XMStoreFloat3(&m_Normals[7], XMVector3Normalize(nor));
	}
	else {
		m_Normals.assign(36, {});
		for (int i = 0; i < 6; ++i) m_Normals[i] = { 0.f, -1.f, 0.f };
		for (int i = 0; i < 6; ++i) m_Normals[i + 6] = { 0.f, 0.f, -1.f };
		for (int i = 0; i < 6; ++i) m_Normals[i + 12] = { 1.f, 0.f, 0.f };
		for (int i = 0; i < 6; ++i) m_Normals[i + 18] = { 0.f, 0.f, 1.f };
		for (int i = 0; i < 6; ++i) m_Normals[i + 24] = { -1.f, 0.f, 0.f };
		for (int i = 0; i < 6; ++i) m_Normals[i + 30] = { 0.f, 1.f, 0.f };
	}

	m_NormalStride = sizeof(XMFLOAT3);
}

CNormalMesh::CNormalMesh(float radius, UINT sampleCount, XMFLOAT3 center)
	: CMesh(radius, sampleCount, center)
{
	if (sampleCount < 8) sampleCount = 8;

	m_Normals.emplace_back(0.f, 1.f, 0.f);

	float vertical = 180.f / (sampleCount + 1);
	float horizon = 360.f / sampleCount;

	for (UINT i = 0; i < sampleCount; ++i) {
		float ver = 90.0f - (vertical * static_cast<float>(i + 1));
		float radver = XMConvertToRadians(ver);
		float horRadius = radius * cosf(radver);
		for (UINT j = 0; j < sampleCount; ++j) {
			float hor = horizon * j;
			float radhor = XMConvertToRadians(hor);
			XMFLOAT3 normal = { horRadius * cosf(radhor) ,radius * sinf(radver),  horRadius * sinf(radhor) };
			XMStoreFloat3(&normal, XMVector3Normalize(XMLoadFloat3(&normal)));
			m_Normals.emplace_back(normal);
		}
	}
	m_Normals.emplace_back(0.f, -1.f, 0.f);

	m_NormalStride = sizeof(XMFLOAT3);
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

CMeshDX11::CMeshDX11(void* device, float radius, UINT sampleCount, XMFLOAT3 center, void* command)
	: CMesh(radius, sampleCount, center)
{
	InitVertexAndIndexBuffers(device, command);
	m_PT = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
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
	if (m_IndexBuffers.size() > 0) {
		for (int i = 0; i < m_IndexBuffers.size(); ++i) {
			context->IASetIndexBuffer(m_IndexBuffers[i].Get(), DXGI_FORMAT_R32_UINT, 0);
			context->DrawIndexed(m_Indices[i].size(), 0, 0);
		}
	}
	else {
		context->DrawInstanced(m_Vertices.size() * 3, 1, 0, 0);
	}
}

void CMeshDX11::Render(void* command, UINT numIndex)
{
	ID3D11DeviceContext* context = reinterpret_cast<ID3D11DeviceContext*>(command);
	context->IASetPrimitiveTopology(m_PT);
	context->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &m_VertexStride, &m_VertexOffset);
	size_t indexSize = m_IndexBuffers.size();
	if (indexSize > 0) {
		if (numIndex >= indexSize) numIndex = indexSize - 1;
		context->IASetIndexBuffer(m_IndexBuffers[numIndex].Get(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(m_Indices[numIndex].size(), 0, 0);
	}
	else {
		context->DrawInstanced(m_Vertices.size() * 3, 1, 0, 0);
	}
}


CNormalMeshDX11::CNormalMeshDX11(void* device, float width, float height, void* command, bool BasedOnViewport)
	: CNormalMesh(width, height, BasedOnViewport)
{
	InitBuffers(device, command);
	m_PT = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

CNormalMeshDX11::CNormalMeshDX11(void* device, float width, float height, float depth, void* command, XMFLOAT3 center, bool haveIndex)
	: CNormalMesh(width, height, depth, center, haveIndex)
{
	InitBuffers(device, command);
	m_PT = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

CNormalMeshDX11::CNormalMeshDX11(void* device, float radius, UINT sampleCount, XMFLOAT3 center, void* command)
	: CNormalMesh(radius, sampleCount, center)
{
	InitBuffers(device, command);
	m_PT = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

void CNormalMeshDX11::InitBuffers(void* device, void* command)
{
	ID3D11Device* dev = reinterpret_cast<ID3D11Device*>(device);

	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = m_VertexStride * m_Vertices.size();
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = m_Vertices.data();
	dev->CreateBuffer(&desc, &data, m_VertexBuffer.GetAddressOf());

	desc.ByteWidth = m_NormalStride * m_Normals.size();
	data.pSysMem = m_Normals.data();
	dev->CreateBuffer(&desc, &data, m_NormalBuffer.GetAddressOf());

	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	for (int i = 0; i < m_Indices.size(); ++i) {
		desc.ByteWidth = sizeof(UINT) * m_Indices[i].size();
		data.pSysMem = m_Indices[i].data();
		ComPtr<ID3D11Buffer> tempBuffer;
		dev->CreateBuffer(&desc, &data, tempBuffer.GetAddressOf());
		m_IndexBuffers.push_back(tempBuffer);
	}
}

void CNormalMeshDX11::Render(void* command)
{
	ID3D11DeviceContext* context = reinterpret_cast<ID3D11DeviceContext*>(command);
	context->IASetPrimitiveTopology(m_PT);
	ID3D11Buffer* buffers[] = { m_VertexBuffer.Get(), m_NormalBuffer.Get() };
	UINT strides[] = { m_VertexStride, m_NormalStride };
	UINT offsets[] = { m_VertexOffset, m_NormalOffset };
	context->IASetVertexBuffers(0, 2, buffers, strides, offsets);
	if (m_IndexBuffers.size() > 0) {
		for (int i = 0; i < m_IndexBuffers.size(); ++i) {
			context->IASetIndexBuffer(m_IndexBuffers[i].Get(), DXGI_FORMAT_R32_UINT, 0);
			context->DrawIndexed(m_Indices[i].size(), 0, 0);
		}
	}
	else {
		context->DrawInstanced(m_Vertices.size() * 3, 1, 0, 0);
	}
}

void CNormalMeshDX11::Render(void* command, UINT numIndex)
{
	ID3D11DeviceContext* context = reinterpret_cast<ID3D11DeviceContext*>(command);
	context->IASetPrimitiveTopology(m_PT);
	ID3D11Buffer* buffers[] = { m_VertexBuffer.Get(), m_NormalBuffer.Get() };
	UINT strides[] = { m_VertexStride, m_NormalStride };
	UINT offsets[] = { m_VertexOffset, m_NormalOffset };
	context->IASetVertexBuffers(0, 2, buffers, strides, offsets);
	size_t indexSize = m_IndexBuffers.size();
	if (indexSize > 0) {
		if (numIndex >= indexSize) numIndex = indexSize - 1;
		context->IASetIndexBuffer(m_IndexBuffers[numIndex].Get(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(m_Indices[numIndex].size(), 0, 0);
	}
	else {
		context->DrawInstanced(m_Vertices.size() * 3, 1, 0, 0);
	}
}