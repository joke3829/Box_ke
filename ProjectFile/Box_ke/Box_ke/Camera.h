
#pragma once
#include "stdafx.h"
class CGameObject;

// Camera Cbuffer	언제든지 변경 가능 
struct CameraCBuffer {
	XMFLOAT4X4	ViewProj;
	XMFLOAT4X4	InvViewProj;
	XMFLOAT3	cameraEye;
};

class CCamera {
public:
	CCamera(float fov, float aspect, float nearZ, float farZ);				// 원근투영
	CCamera(UINT viewWidth, UINT viewHeight, float nearZ, float farZ);		// 직교투영

	virtual void UpdateCameraBuffer(void* command) {}
	virtual void SetShaderVariable(void* command, ShaderType type = ST_NONE) {}	// DX11의 경우 반드시 2번째 인자를 채워야한다.

	void SetStartSlot(UINT slot);
	void SetRootParameterIndex(UINT index);

	void ModifyProjection(float fov, float aspect, float nearZ, float farZ);
	void ModifyProjection(UINT viewWidth, UINT viewHeight, float nearZ, float farZ);

	void SetThirdPersonCamera(bool state);
	void SetTarget(CGameObject* target);
	void ToggleCameraState();

	void SetCameraEYE(float x, float y, float z);
	void SetCameraEYE(XMFLOAT3 eye);

	XMFLOAT3 GetEYE() const;
	XMFLOAT3 GetLookVec() const;
	XMFLOAT3 GetRightVec() const;
	XMFLOAT3 GetUpVec() const;

	XMMATRIX GetProjMatrix() const;

	void MakeRightVec();

	void Move(float speed, float elapsedTime, XMFLOAT3 arrow = {});
	void Rotate(float deltaX, float deltaY);
protected:
	union {
		UINT			m_StartSlot{};
		UINT			m_RootParameterIndex;
	};

	XMFLOAT3		m_EYE{};
	XMFLOAT3		m_LookVec{ 0.f, 0.f, 1.f };
	XMFLOAT3		m_UpVec{ 0.f, 1.f, 0.f };
	XMFLOAT3		m_RightVec{};

	XMMATRIX		m_ProjectionMatrix{};

	// 3인칭 옵션
	bool			m_bThirdPerson{};
	XMFLOAT3		m_OffsetVec{ 0.f, 0.f, -1.f };
	CGameObject*	m_Target{};
};

// DX11 ==============================================================

class CCameraDX11 : public CCamera {
public:
	CCameraDX11(void* device, float fov, float aspect, float nearZ, float farZ);
	CCameraDX11(void* device, UINT viewWidth, UINT viewHeight, float nearZ, float farZ);

	void UpdateCameraBuffer(void* command);
	void SetShaderVariable(void* command, ShaderType type = ST_NONE);
protected:
	ComPtr<ID3D11Buffer>		m_CameraBuffer{};
};