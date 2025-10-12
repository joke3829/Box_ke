// ==========================================================
// Scene.h
// ��� ������ �ۼ��� ���
// Viewport ������ Scene�� �������� ����
// ==========================================================

#pragma once
#include "stdafx.h"
#include "Camera.h"
#include "MyShader.h"
#include "Object.h"

class CScene {
public:
	// command: DX11 -> DeviceContext, DX12 -> CommandList
	virtual void Setup(void* device, void* command = nullptr) {}

	virtual void ProcessInput(float elapsedTime) {}
	virtual void KeyboardMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {}
	virtual void MouseMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {}

	virtual void UpdateObject(float elapsedTime, void* command = nullptr) {}

	virtual void PreRender(void* command = nullptr) {}
	virtual void Render(void* command = nullptr) {}
	virtual void PostRender(void* command = nullptr) {}
};

class CSceneDX11 : public CScene {
public:
	virtual void Setup(void* device, void* command = nullptr) {}

	virtual void ProcessInput(float elapsedTime) {}
	virtual void KeyboardMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {}
	virtual void MouseMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {}

	virtual void UpdateObject(float elapsedTime, void* command = nullptr) {}

	virtual void PreRender(void* command = nullptr) {}
	virtual void Render(void* command = nullptr) {}
	virtual void PostRender(void* command = nullptr) {}
};

