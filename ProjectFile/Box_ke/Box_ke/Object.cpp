#include "Object.h"

void CGameObject::UpdateWorldMat()
{
	XMMATRIX scaleMat = XMMatrixScaling(m_ScaleFactor.x, m_ScaleFactor.y, m_ScaleFactor.z);

	XMFLOAT4X4 rotMat{};
	rotMat._11 = m_RightVec.x; rotMat._12 = m_RightVec.y; rotMat._13 = m_RightVec.z; rotMat._14 = 0;
	rotMat._21 = m_UpVec.x; rotMat._22 = m_UpVec.y; rotMat._23 = m_UpVec.z; rotMat._24 = 0;
	rotMat._31 = m_LookVec.x; rotMat._32 = m_LookVec.y; rotMat._33 = m_LookVec.z; rotMat._34 = 0;
	rotMat._41 = 0; rotMat._42 = 0; rotMat._43 = 0; rotMat._44 = 1;
	XMStoreFloat4x4(&m_WorldMat, scaleMat * XMLoadFloat4x4(&rotMat));
	m_WorldMat._41 = m_Position.x; m_WorldMat._42 = m_Position.y; m_WorldMat._43 = m_Position.z;
}

void CGameObject::SetMesh(std::shared_ptr<CMesh> mesh)
{
	m_Mesh = mesh;
}

// 정상 작동하는지 확인 안해봄, 사용 권장 x
void CGameObject::SetMaterials(std::vector<std::shared_ptr<CMaterial>> materials)
{
	m_Materials = materials;
}

void CGameObject::SetScaleFactor(float x, float y, float z)
{
	m_ScaleFactor = { x, y, z };
}

void CGameObject::SetScaleFactor(XMFLOAT3 scale)
{
	m_ScaleFactor = scale;
}

void CGameObject::SetPosition(float x, float y, float z)
{
	m_Position = { x, y, z };
}

void CGameObject::SetPosition(XMFLOAT3 pos)
{
	m_Position = pos;
}



void CGameObject::SetStartSlot(UINT slot)
{
	m_StartSlot = slot;
}

void CGameObject::SetRootParameterIndex(UINT index)
{
	m_RootParameterIndex = index;
}

XMFLOAT3 CGameObject::GetRightVec() const
{
	return m_RightVec;
}

XMFLOAT3 CGameObject::GetUpVec() const
{
	return m_UpVec;
}

XMFLOAT3 CGameObject::GetLookVec() const
{
	return m_LookVec;
}

XMFLOAT3 CGameObject::GetPosition() const
{
	return m_Position;
}

XMFLOAT3 CGameObject::GetRotation() const
{
	
	return m_Rotation;
}

XMFLOAT3 CGameObject::GetScaleFactor() const
{
	return m_ScaleFactor;
}

XMFLOAT4X4 CGameObject::GetWorldMatrix()
{
	UpdateWorldMat();		// 월드 행렬은 최신화 해서 넘겨준다.
	return m_WorldMat;
}

std::vector<std::shared_ptr<CMaterial>>& CGameObject::GetMaterials()
{
	return m_Materials;
}

// X, Y, Z 절대 축을 이용한 회전
// 회전은 x, y, z 순으로 진행한다.
void CGameObject::RotateAbsAxis(float AxisX, float AxisY, float AxisZ)
{
	if (AxisX != 0.f) {
		XMStoreFloat3(&m_RightVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_RightVec), XMMatrixRotationX(XMConvertToRadians(AxisX)))));
		XMStoreFloat3(&m_UpVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_UpVec), XMMatrixRotationX(XMConvertToRadians(AxisX)))));
		XMStoreFloat3(&m_LookVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_LookVec), XMMatrixRotationX(XMConvertToRadians(AxisX)))));
	}
	if (AxisY != 0.f) {
		XMStoreFloat3(&m_RightVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_RightVec), XMMatrixRotationY(XMConvertToRadians(AxisY)))));
		XMStoreFloat3(&m_UpVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_UpVec), XMMatrixRotationY(XMConvertToRadians(AxisY)))));
		XMStoreFloat3(&m_LookVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_LookVec), XMMatrixRotationY(XMConvertToRadians(AxisY)))));
	}
	if (AxisZ != 0.f) {
		XMStoreFloat3(&m_RightVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_RightVec), XMMatrixRotationZ(XMConvertToRadians(AxisZ)))));
		XMStoreFloat3(&m_UpVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_UpVec), XMMatrixRotationZ(XMConvertToRadians(AxisZ)))));
		XMStoreFloat3(&m_LookVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_LookVec), XMMatrixRotationZ(XMConvertToRadians(AxisZ)))));
	}
}

void CGameObject::RotateAbsAxis(XMFLOAT3 rot)
{
	if (rot.x != 0.f) {
		XMStoreFloat3(&m_RightVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_RightVec), XMMatrixRotationX(XMConvertToRadians(rot.x)))));
		XMStoreFloat3(&m_UpVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_UpVec), XMMatrixRotationX(XMConvertToRadians(rot.x)))));
		XMStoreFloat3(&m_LookVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_LookVec), XMMatrixRotationX(XMConvertToRadians(rot.x)))));
	}
	if (rot.y != 0.f) {
		XMStoreFloat3(&m_RightVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_RightVec), XMMatrixRotationY(XMConvertToRadians(rot.y)))));
		XMStoreFloat3(&m_UpVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_UpVec), XMMatrixRotationY(XMConvertToRadians(rot.y)))));
		XMStoreFloat3(&m_LookVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_LookVec), XMMatrixRotationY(XMConvertToRadians(rot.y)))));
	}
	if (rot.z != 0.f) {
		XMStoreFloat3(&m_RightVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_RightVec), XMMatrixRotationZ(XMConvertToRadians(rot.z)))));
		XMStoreFloat3(&m_UpVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_UpVec), XMMatrixRotationZ(XMConvertToRadians(rot.z)))));
		XMStoreFloat3(&m_LookVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_LookVec), XMMatrixRotationZ(XMConvertToRadians(rot.z)))));
	}
}

// 객체의 각 축을 이용한 회전
// 회전은 right, up, look 순으로 진행한다.
void CGameObject::RotateLocalAxis(float right, float up, float look)
{
	if (right != 0.f) {

		XMStoreFloat3(&m_UpVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_UpVec), XMMatrixRotationAxis(XMLoadFloat3(&m_RightVec), XMConvertToRadians(right)))));
		XMStoreFloat3(&m_LookVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_LookVec), XMMatrixRotationAxis(XMLoadFloat3(&m_RightVec), XMConvertToRadians(right)))));
	}
	if (up != 0.f) {

		XMStoreFloat3(&m_RightVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_RightVec), XMMatrixRotationAxis(XMLoadFloat3(&m_UpVec), XMConvertToRadians(up)))));
		XMStoreFloat3(&m_LookVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_LookVec), XMMatrixRotationAxis(XMLoadFloat3(&m_UpVec), XMConvertToRadians(up)))));
	}
	if (look != 0.f) {

		XMStoreFloat3(&m_RightVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_RightVec), XMMatrixRotationAxis(XMLoadFloat3(&m_LookVec), XMConvertToRadians(look)))));
		XMStoreFloat3(&m_UpVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_UpVec), XMMatrixRotationAxis(XMLoadFloat3(&m_LookVec), XMConvertToRadians(look)))));
	}

}

void CGameObject::RotateLocalAxis(XMFLOAT3 rot)
{
	if (rot.x != 0.f) {
		
		XMStoreFloat3(&m_UpVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_UpVec), XMMatrixRotationAxis(XMLoadFloat3(&m_RightVec), XMConvertToRadians(rot.x)))));
		XMStoreFloat3(&m_LookVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_LookVec), XMMatrixRotationAxis(XMLoadFloat3(&m_RightVec), XMConvertToRadians(rot.x)))));
	}
	if (rot.y != 0.f) {
		
		XMStoreFloat3(&m_RightVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_RightVec), XMMatrixRotationAxis(XMLoadFloat3(&m_UpVec), XMConvertToRadians(rot.y)))));
		XMStoreFloat3(&m_LookVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_LookVec), XMMatrixRotationAxis(XMLoadFloat3(&m_UpVec), XMConvertToRadians(rot.y)))));
	}
	if (rot.z != 0.f) {
		
		XMStoreFloat3(&m_RightVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_RightVec), XMMatrixRotationAxis(XMLoadFloat3(&m_LookVec), XMConvertToRadians(rot.z)))));
		XMStoreFloat3(&m_UpVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_UpVec), XMMatrixRotationAxis(XMLoadFloat3(&m_LookVec), XMConvertToRadians(rot.z)))));
	}


}

void CGameObject::RotateLocalAxis(XMFLOAT4& quat)
{
	XMMATRIX rotMat = XMMatrixRotationQuaternion(XMLoadFloat4(&quat));

	XMFLOAT3 right, up, look;
	XMStoreFloat3(&right, rotMat.r[0]);
	XMStoreFloat3(&up, rotMat.r[1]);
	XMStoreFloat3(&look, rotMat.r[2]);

	m_RightVec = right;
	m_UpVec = up;
	m_LookVec = look;
}

void CGameObject::ResetWorldMat()
{
	m_RightVec = { 1.f, 0.f, 0.f };
	m_UpVec = { 0.f, 1.f, 0.f };
	m_LookVec = { 0.f, 0.f, 1.f };
	m_Position = {};

	m_ScaleFactor = { 1.f, 1.f, 1.f };
}

// CGameObjectDX11 ===================================================================

CGameObjectDX11::CGameObjectDX11(void* device)
{
	ID3D11Device* dev = reinterpret_cast<ID3D11Device*>(device);

	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = Align(sizeof(XMFLOAT4X4), 16);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	dev->CreateBuffer(&desc, nullptr, m_WorldBuffer.GetAddressOf());
}

void CGameObjectDX11::Render(void* command)
{
	UpdateWorldMat();
	if (m_Mesh) {
		ID3D11DeviceContext* context = reinterpret_cast<ID3D11DeviceContext*>(command);
		D3D11_MAPPED_SUBRESOURCE data{};
		context->Map(m_WorldBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
		XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(data.pData), XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMat)));
		context->Unmap(m_WorldBuffer.Get(), 0);
		context->VSSetConstantBuffers(m_StartSlot, 1, m_WorldBuffer.GetAddressOf());
		for (UINT i = 0; i < m_Materials.size(); ++i) {			// Material의 개수만큼 렌더
			m_Materials[i]->SetShaderVariable(command, ST_PS);	// PS에 Set
			m_Mesh->Render(command, i);
		}
	}
}

// CHierarchyGameObjectDX11=======================================================================================

CHierarchyGameObjectDX11::CHierarchyGameObjectDX11(void* device)
	: CGameObjectDX11(device)
{ 
	XMStoreFloat4x4(&m_ParentWorldMat, XMMatrixIdentity());
}

void CHierarchyGameObjectDX11::UpdateWorldMat()
{
	CGameObjectDX11::UpdateWorldMat();
	XMStoreFloat4x4(&m_HierarchyWorldMat, XMLoadFloat4x4(&m_WorldMat) * XMLoadFloat4x4(&m_ParentWorldMat));
	for (std::shared_ptr<CGameObject>& child : m_Childs) {
		auto* p = dynamic_cast<CHierarchyGameObjectDX11*>(child.get());
		p->SetParentMat(m_HierarchyWorldMat);
		p->UpdateWorldMat();
	}
}

void CHierarchyGameObjectDX11::SetParentMat(XMFLOAT4X4 mat)
{
	m_ParentWorldMat = mat;
}

XMFLOAT4X4 CHierarchyGameObjectDX11::GetHierarchyWorldMat()
{
	UpdateWorldMat();
	return m_HierarchyWorldMat;
}

std::vector<std::shared_ptr<CGameObject>>& CHierarchyGameObjectDX11::GetChilds()
{
	return m_Childs;
}

void CHierarchyGameObjectDX11::Render(void* command)
{
	CGameObject::UpdateWorldMat();
	XMStoreFloat4x4(&m_HierarchyWorldMat, XMLoadFloat4x4(&m_WorldMat) * XMLoadFloat4x4(&m_ParentWorldMat));
	if (m_Mesh) {
		ID3D11DeviceContext* context = reinterpret_cast<ID3D11DeviceContext*>(command);
		D3D11_MAPPED_SUBRESOURCE data{};
		context->Map(m_WorldBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
		XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(data.pData), XMMatrixTranspose(XMLoadFloat4x4(&m_HierarchyWorldMat)));
		context->Unmap(m_WorldBuffer.Get(), 0);
		context->VSSetConstantBuffers(m_StartSlot, 1, m_WorldBuffer.GetAddressOf());
		for (UINT i = 0; i < m_Materials.size(); ++i) {			// Material의 개수만큼 렌더
			m_Materials[i]->SetShaderVariable(command, ST_PS);	// PS에 Set
			m_Mesh->Render(command, i);
		}
	}
	for (std::shared_ptr<CGameObject>& child : m_Childs) {
		auto* p = dynamic_cast<CHierarchyGameObjectDX11*>(child.get());
		p->SetParentMat(m_HierarchyWorldMat);
		p->Render(command);
	}
}