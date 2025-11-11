#include "Light.h"

CLight::CLight(LightCB& cb, XMFLOAT4 lightColor, XMFLOAT3 direction, float intensity, UINT index)
	: m_LightInfo{ cb }
{
	cb.type = LT_DIRECTIONAL;
	cb.lightColor = lightColor;
	cb.direction = direction;
	cb.intensity = intensity;
	cb.index = index;

	m_InitialDir = direction;
	// temp
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(DEFAULT_SHADOWMAP_SIZE, DEFAULT_SHADOWMAP_SIZE, 0.001f, 500.f));

	XMStoreFloat4x4(&m_LocalMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
}

CLight::CLight(LightCB& cb, XMFLOAT3 pos, XMFLOAT4 lightColor, XMFLOAT3 direction, float intensity, float range, float spotAngle, UINT index)
	: m_LightInfo{ cb }
{
	cb.type = LT_SPOT;
	cb.position = pos;
	cb.lightColor = lightColor;
	cb.direction = direction;
	cb.intensity = intensity;
	cb.range = range;
	cb.spotAngle = spotAngle;
	cb.index = index;

	m_Position = pos;
	m_InitialDir = direction;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixPerspectiveFovLH(spotAngle, 1.f, 0.001f, range));

	XMStoreFloat4x4(&m_LocalMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
}

CLight::CLight(LightCB& cb, XMFLOAT3 pos, XMFLOAT4 lightColor, float intensity, float range, UINT index)
	: m_LightInfo{ cb }
{
	cb.type = LT_POINT;
	cb.position = pos;
	cb.lightColor = lightColor;
	cb.intensity = intensity;
	cb.range = range;
	cb.index = index;

	m_Position = pos;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixPerspectiveFovLH(90.f, 1.f, 0.001f, range));

	XMStoreFloat4x4(&m_LocalMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
}

void CLight::SetParentObject(CGameObject* target)
{
	m_ParentObject = target;
}

void CLight::UpdateLightInfo()
{
	m_LightInfo.position = m_Position;
	if (m_LightInfo.type != LT_POINT) {
		XMStoreFloat3(&m_LightInfo.direction, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_InitialDir), XMLoadFloat4x4(&m_WorldMatrix))));
	}
}

void CLight::UpdateMatrix()	// 부정확
{
	m_LocalMatrix._11 = m_Right.x; m_LocalMatrix._12 = m_Right.y; m_LocalMatrix._13 = m_Right.z; m_LocalMatrix._14 = 0;
	m_LocalMatrix._21 = m_Up.x; m_LocalMatrix._22 = m_Up.y; m_LocalMatrix._23 = m_Up.z; m_LocalMatrix._24 = 0;
	m_LocalMatrix._31 = m_Look.x; m_LocalMatrix._32 = m_Look.y; m_LocalMatrix._33 = m_Look.z; m_LocalMatrix._34 = 0;
	m_LocalMatrix._41 = m_Position.x; m_LocalMatrix._42 = m_Position.y; m_LocalMatrix._43 = m_Position.z; m_LocalMatrix._44 = 1;
	if (m_ParentObject) {
		XMFLOAT4X4 parent = m_ParentObject->GetWorldMatrix();
		XMStoreFloat4x4(&m_WorldMatrix, XMLoadFloat4x4(&m_LocalMatrix) * XMLoadFloat4x4(&parent));
	}
	else
		m_WorldMatrix = m_LocalMatrix;

	UpdateLightInfo();
}

// =======================================================================================

CLightDX11::CLightDX11(ID3D11Device* device, LightCB& cb, XMFLOAT4 lightColor, XMFLOAT3 direction, float intensity, UINT index)
	: CLight(cb, lightColor, direction, intensity, index)
{
	// temp
	m_Viewport = { 0, 0, DEFAULT_SHADOWMAP_SIZE, DEFAULT_SHADOWMAP_SIZE, 0.f, 1.f };
}

CLightDX11::CLightDX11(ID3D11Device* device, LightCB& cb, XMFLOAT3 pos, XMFLOAT4 lightColor, XMFLOAT3 direction, float intensity, float range, float spotAngle, UINT index)
	: CLight(cb, pos, lightColor, direction, intensity, range, spotAngle, index)
{
	m_Viewport = { 0, 0, DEFAULT_SHADOWMAP_SIZE, DEFAULT_SHADOWMAP_SIZE, 0.f, 1.f };
}

CLightDX11::CLightDX11(ID3D11Device* device, LightCB& cb, XMFLOAT3 pos, XMFLOAT4 lightColor, float intensity, float range, UINT index)
	: CLight(cb, pos, lightColor, intensity, range, index)
{
	m_Viewport = { 0, 0, DEFAULT_SHADOWMAP_SIZE, DEFAULT_SHADOWMAP_SIZE, 0.f, 1.f };
}

void CLightDX11::UpdateMatrix()
{
	m_LocalMatrix._11 = m_Right.x; m_LocalMatrix._12 = m_Right.y; m_LocalMatrix._13 = m_Right.z; m_LocalMatrix._14 = 0;
	m_LocalMatrix._21 = m_Up.x; m_LocalMatrix._22 = m_Up.y; m_LocalMatrix._23 = m_Up.z; m_LocalMatrix._24 = 0;
	m_LocalMatrix._31 = m_Look.x; m_LocalMatrix._32 = m_Look.y; m_LocalMatrix._33 = m_Look.z; m_LocalMatrix._34 = 0;
	m_LocalMatrix._41 = m_Position.x; m_LocalMatrix._42 = m_Position.y; m_LocalMatrix._43 = m_Position.z; m_LocalMatrix._44 = 1;
	if (m_ParentObject) {
		CHierarchyGameObjectDX11* p = dynamic_cast<CHierarchyGameObjectDX11*>(m_ParentObject);
		XMFLOAT4X4 parent;
		if (p) parent = p->GetHierarchyWorldMat();
		else parent = m_ParentObject->GetWorldMatrix();
		XMStoreFloat4x4(&m_WorldMatrix, XMLoadFloat4x4(&m_LocalMatrix) * XMLoadFloat4x4(&parent));
	}
	else
		m_WorldMatrix = m_LocalMatrix;

	CLight::UpdateLightInfo();
}

void CLightDX11::UpdateShadowMap(void* command, bool useCameraPos, XMFLOAT3 cameraPos)
{

}