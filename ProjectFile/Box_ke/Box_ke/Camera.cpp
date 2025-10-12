#include "Camera.h"

CCamera::CCamera(float fov, float aspect, float nearZ, float farZ)
{
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), aspect, nearZ, farZ);
}

CCamera::CCamera(UINT viewWidth, UINT viewHeight, float nearZ, float farZ)
{
	m_ProjectionMatrix = XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ);
}

void CCamera::SetStartSlot(UINT slot)
{
	m_StartSlot = slot;
}

void CCamera::ModifyProjection(float fov, float aspect, float nearZ, float farZ)
{
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), aspect, nearZ, farZ);
}

void CCamera::ModifyProjection(UINT viewWidth, UINT viewHeight, float nearZ, float farZ)
{
	m_ProjectionMatrix = XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ);
}

void CCamera::SetThirdPersonCamera(bool state)
{
	m_bThirdPerson = state;
}

void CCamera::SetTarget(CGameObject* target)
{
	m_Target = target;
}

void CCamera::ToggleCameraState()
{
	m_bThirdPerson = !m_bThirdPerson;
}

void CCamera::SetCameraEYE(float x, float y, float z)
{
	m_EYE = { x, y ,z };
}

void CCamera::SetCameraEYE(XMFLOAT3 eye)
{
	m_EYE = eye;
}

XMFLOAT3 CCamera::GetEYE() const
{
	return m_EYE;
}

XMFLOAT3 CCamera::GetLookVec() const
{
	return m_LookVec;
}

XMFLOAT3 CCamera::GetRightVec() const
{
	return m_RightVec;
}

void CCamera::MakeRightVec()
{
	XMStoreFloat3(&m_RightVec, XMVector3Cross(XMLoadFloat3(&m_UpVec), XMLoadFloat3(&m_LookVec)));
}

void CCamera::Move(float speed, float elapsedTime, XMFLOAT3 arrow)
{
	XMVECTOR look{};
	if (arrow.x == 0.f && arrow.y == 0.f && arrow.z == 0.f) look = XMLoadFloat3(&m_LookVec);
	else look = XMLoadFloat3(&arrow);

	XMVECTOR eye = XMLoadFloat3(&m_EYE);
	XMStoreFloat3(&m_EYE, eye + (look * speed * elapsedTime));
}

void CCamera::Rotate(float deltaX, float deltaY)
{
	XMMATRIX rotMat = XMMatrixIdentity();
	if (deltaX != 0.0) {
		XMMATRIX xMat = XMMatrixRotationAxis(XMLoadFloat3(&m_UpVec), XMConvertToRadians(deltaX));
		rotMat = rotMat * xMat;
	}
	if (deltaY != 0.0) {
		XMVECTOR right = XMVector3Cross(XMLoadFloat3(&m_UpVec), XMLoadFloat3(&m_LookVec));
		XMMATRIX yMat = XMMatrixRotationAxis(right, XMConvertToRadians(deltaY));
		rotMat = rotMat * yMat;
	}
	XMStoreFloat3(&m_LookVec, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_LookVec), rotMat)));
}

// ========================================================

CCameraDX11::CCameraDX11(void* device, float fov, float aspect, float nearZ, float farZ)
	: CCamera(fov, aspect, nearZ, farZ)
{
	ID3D11Device* dev = reinterpret_cast<ID3D11Device*>(device);

	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = Align(sizeof(CameraCBuffer), 16);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	dev->CreateBuffer(&desc, nullptr, m_CameraBuffer.GetAddressOf());
}

CCameraDX11::CCameraDX11(void* device, UINT viewWidth, UINT viewHeight, float nearZ, float farZ)
	: CCamera(viewWidth, viewHeight, nearZ, farZ)
{
	ID3D11Device* dev = reinterpret_cast<ID3D11Device*>(device);

	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = Align(sizeof(CameraCBuffer), 16);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	dev->CreateBuffer(&desc, nullptr, m_CameraBuffer.GetAddressOf());
}

void CCameraDX11::UpdateCameraBuffer(void* command)
{
	ID3D11DeviceContext* context = reinterpret_cast<ID3D11DeviceContext*>(command);
	XMMATRIX Mat;
	XMMATRIX viewProj;
	if (m_bThirdPerson && m_Target) {	// 변경 예정
		Mat = XMMatrixLookToLH(XMLoadFloat3(&m_EYE), XMLoadFloat3(&m_LookVec), XMLoadFloat3(&m_UpVec));
		viewProj = Mat * m_ProjectionMatrix;
		Mat = XMMatrixInverse(nullptr, viewProj);
	}
	else {
		Mat = XMMatrixLookToLH(XMLoadFloat3(&m_EYE), XMLoadFloat3(&m_LookVec), XMLoadFloat3(&m_UpVec));
		viewProj = Mat * m_ProjectionMatrix;
		Mat = XMMatrixInverse(nullptr, viewProj);
	}

	viewProj = XMMatrixTranspose(viewProj);
	Mat = XMMatrixTranspose(viewProj);
	D3D11_MAPPED_SUBRESOURCE mapped{};
	context->Map(m_CameraBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	CameraCBuffer* p = reinterpret_cast<CameraCBuffer*>(mapped.pData);
	XMStoreFloat4x4(&(p->ViewProj), viewProj);
	XMStoreFloat4x4(&(p->InvViewProj), Mat);
	context->Unmap(m_CameraBuffer.Get(), 0);
}

void CCameraDX11::SetShaderVariable(void* command, ShaderType type)
{
	switch (type) {
	case ST_VS:
		reinterpret_cast<ID3D11DeviceContext*>(command)->VSSetConstantBuffers(m_StartSlot, 1, m_CameraBuffer.GetAddressOf());
		break;
	case ST_HS:
		reinterpret_cast<ID3D11DeviceContext*>(command)->HSSetConstantBuffers(m_StartSlot, 1, m_CameraBuffer.GetAddressOf());
		break;
	case ST_DS:
		reinterpret_cast<ID3D11DeviceContext*>(command)->DSSetConstantBuffers(m_StartSlot, 1, m_CameraBuffer.GetAddressOf());
		break;
	case ST_GS:
		reinterpret_cast<ID3D11DeviceContext*>(command)->GSSetConstantBuffers(m_StartSlot, 1, m_CameraBuffer.GetAddressOf());
		break;
	case ST_PS:
		reinterpret_cast<ID3D11DeviceContext*>(command)->PSSetConstantBuffers(m_StartSlot, 1, m_CameraBuffer.GetAddressOf());
		break;
	case ST_CS:
		reinterpret_cast<ID3D11DeviceContext*>(command)->CSSetConstantBuffers(m_StartSlot, 1, m_CameraBuffer.GetAddressOf());
		break;
	default:
		MessageBoxA(0, "Insert Invalid Type", "Fatal Error_DX11", MB_OK);
		break;
	}
}