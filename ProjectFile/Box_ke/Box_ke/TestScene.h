#pragma once
#include "Scene.h"
#include "MyObject.h"
#include "Animation.h"
#include "CDebugLineDX11.h"


class CHeroScene : public CScene
{
public:
	void Setup(void* device, void* command = nullptr);

	void ProcessInput(float elapsedTime);
	void KeyboardMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void MouseMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void UpdateObject(float elapsedTime, void* command = nullptr);

	void PickingMouse(HWND hWnd);

	void Render(void* command = nullptr);
protected:
	std::unique_ptr<CCamera>		m_Camera{};
	std::unique_ptr<CShader>		m_Shader{};

	D3D11_VIEWPORT					m_Viewport{};
	std::vector<std::shared_ptr<CGameObject>>	m_Objects{};

	// 이거 귀찮아서 그냥 멤버 변수로 빼놨슴다
	std::shared_ptr<CHierarchyGameObjectDX11> m_Gun{};

	POINT							oldCursor{};
	bool							click{};

	ID3D11Device* tempdev{};

	std::shared_ptr<CGameObject> m_Hero{};


protected:


	std::unique_ptr<CDebugLineDX11> m_DebugLaser;
	bool m_bDrawLaser = false;

	XMFLOAT3 m_LaserStart{};
	XMFLOAT3 m_LaserEnd{};
	XMFLOAT3 m_AimPoint{};


};

