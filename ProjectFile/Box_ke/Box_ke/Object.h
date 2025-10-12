// ====================================================
// Object.h
// Object 정보를 정리한 헤더
// 
// CGameObject: 단일 객체 구조
// CHierarchyGameObject: 자식을 가지는 계층 구조
// ====================================================
#pragma once
#include "stdafx.h"
#include "Mesh.h"

class CGameObject {
public:
	CGameObject() {}
	virtual void UpdateObject(float elapsedTime) {}
	virtual void UpdateWorldMat();

	void SetMesh(std::shared_ptr<CMesh> mesh);

	void SetScaleFactor(float x, float y, float z);
	void SetScaleFactor(XMFLOAT3 scale);

	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 pos);

	void SetStartSlot(UINT slot);

	XMFLOAT3 GetRightVec() const;
	XMFLOAT3 GetUpVec() const;
	XMFLOAT3 GetLookVec() const;
	XMFLOAT3 GetPosition() const;
	XMFLOAT3 GetScaleFactor() const;
	XMFLOAT4X4 GetWorldMatrix();

	// 바운딩 박스 추가 예정

	void RotateAbsAxis(float AxisX = 0.f, float AxisY = 0.f, float AxisZ = 0.f);
	void RotateAbsAxis(XMFLOAT3 rot = {});

	void RotateLocalAxis(float right = 0.f, float up = 0.f, float look = 0.f);
	void RotateLocalAxis(XMFLOAT3 rot = {});

	void ResetWorldMat();

	virtual void Render(void* command) {}
protected:
	XMFLOAT4X4					m_WorldMat{};

	XMFLOAT3					m_RightVec{ 1.f, 0.f, 0.f };
	XMFLOAT3					m_UpVec{ 0.f, 1.f, 0.f };
	XMFLOAT3					m_LookVec{ 0.f, 0.f, 1.f };
	XMFLOAT3					m_Position{};

	XMFLOAT3					m_ScaleFactor{ 1.f, 1.f, 1.f };	// 각각 xyz 스케일 정도

	UINT						m_StartSlot{};

	// Material 추가 예정

	std::shared_ptr<CMesh>		m_Mesh{};
};

class CGameObjectDX11 : public CGameObject {
public:
	CGameObjectDX11(void* device);
	virtual void UpdateObject(float elapsedTime) {}

	virtual void Render(void* command);
protected:
	ComPtr<ID3D11Buffer>		m_WorldBuffer{};
};

class CHierarchyGameObjectDX11 : public CGameObjectDX11 {
public:
	CHierarchyGameObjectDX11(void* device);

	virtual void UpdateObject(float elapsedTime) {}
	virtual void UpdateWorldMat();

	void SetParentMat(XMFLOAT4X4 mat);

	XMFLOAT4X4 GetHierarchyWorldMat();
	std::vector<std::shared_ptr<CHierarchyGameObjectDX11>>& GetChilds();

	void Render(void* command);
protected:
	XMFLOAT4X4								m_ParentWorldMat{};
	XMFLOAT4X4								m_HierarchyWorldMat{};
	std::vector<std::shared_ptr<CHierarchyGameObjectDX11>>	m_Childs{};
};