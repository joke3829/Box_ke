#pragma once
#include "Scene.h"
#include "MyObject.h"

class CHeroScene : public CScene
{
public:
	void Setup(void* device, void* command = nullptr);

	void ProcessInput(float elapsedTime);
	void KeyboardMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void MouseMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void UpdateObject(float elapsedTime, void* command = nullptr);

	void Render(void* command = nullptr);
protected:
	std::unique_ptr<CCamera>		m_Camera{};
	std::unique_ptr<CShader>		m_Shader{};

	D3D11_VIEWPORT					m_Viewport{};
	std::vector<std::shared_ptr<CGameObject>>	m_Objects{};

	POINT							oldCursor{};
	bool							click{};

	ID3D11Device* tempdev{};

};

