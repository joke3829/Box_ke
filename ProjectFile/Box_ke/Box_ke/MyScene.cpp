#include "MyScene.h"

void CTestSceneDX11::Setup(void* device, void* command)
{
	ID3D11Device* dev = reinterpret_cast<ID3D11Device*>(device);
	tempdev = dev;
	m_Shader = std::make_unique<CShaderDX11TestShader>(dev);

	m_Camera = std::make_unique<CCameraDX11>(device, 60.f, 16.f / 9.f, 0.01f, 500.f);
	m_Camera->SetStartSlot(1);
	m_Camera->SetCameraEYE(0.f, 0.f, -30.f);

	m_Viewport = { 0.f, 0.f, 1920.f, 1080.f, 0.f, 1.f };

	std::shared_ptr<CMesh> mesh0 = std::make_shared<CMeshDX11>(device, 5.f, 5.f, 5.f);

	{
		std::shared_ptr<CGameObject> sp = std::make_shared<COrbitObjectDX11>(device);
		m_Objects.push_back(sp);
		auto* p = dynamic_cast<COrbitObjectDX11*>(sp.get());
		p->SetMesh(mesh0);

		auto& c = p->GetChilds();
		std::shared_ptr<CHierarchyGameObjectDX11> cp = std::make_shared<CCircleObjectDX11>(device);
		c.push_back(cp);
		auto& cc = cp->GetChilds();
		
		cp = std::make_shared<COrbitObjectDX11>(device);
		cp->SetMesh(mesh0);
		cp->SetPosition(20.f, 0.f, 0.f);
		cc.push_back(cp);

		cp = std::make_shared<CCircleObjectDX11>(device);
		c.push_back(cp);
		dynamic_cast<CCircleObjectDX11*>(cp.get())->Roll();
		auto& cd = cp->GetChilds();

		cp = std::make_shared<COrbitObjectDX11>(device);
		cp->SetMesh(mesh0);
		cp->SetPosition(10.f, 0.f, 0.f);
		cd.push_back(cp);
	}
}

void CTestSceneDX11::ProcessInput(float elapsedTime)
{
	BYTE keyboard[256];
	BOOL ret = GetKeyboardState(keyboard);

	m_Camera->MakeRightVec();
	if (keyboard['W'] & 0xf0)
		m_Camera->Move(5.0f, elapsedTime);
	if (keyboard['S'] & 0xf0) {
		XMFLOAT3 look = m_Camera->GetLookVec();
		look.x *= -1; look.y *= -1; look.z *= -1;
		m_Camera->Move(5.0f, elapsedTime, look);
	}
	if (keyboard['D'] & 0xf0) {
		m_Camera->Move(5.f, elapsedTime, m_Camera->GetRightVec());
	}
	if (keyboard['A'] & 0xf0) {
		XMFLOAT3 right = m_Camera->GetRightVec();
		right.x *= -1; right.y *= -1; right.z *= -1;
		m_Camera->Move(5.f, elapsedTime, right);
	}
	if (keyboard[VK_SPACE] & 0xf0) {
		m_Camera->Move(5.f, elapsedTime, XMFLOAT3(0.f, 1.f, 0.f));
	}
	if (keyboard[VK_CONTROL] & 0xf0) {
		m_Camera->Move(5.f, elapsedTime, XMFLOAT3(0.f, -1.f, 0.f));
	}
}

void CTestSceneDX11::KeyboardMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_KEYDOWN:
		switch (wParam) {
		case '1':
			m_Shader->ShaderReCompile(tempdev);
			break;
		}
		break;
	}
}

void CTestSceneDX11::MouseMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_LBUTTONDOWN:
		if (!click) {
			click = true;
			GetCursorPos(&oldCursor);
		}
		break;
	case WM_LBUTTONUP:
		if (click) {
			click = false;
		}
		break;
	case WM_MOUSEMOVE:
		if (click) {
			POINT curPos{};
			GetCursorPos(&curPos);

			float deltaX = static_cast<float>(curPos.x - oldCursor.x) / 3.f;
			float deltaY = static_cast<float>(curPos.y - oldCursor.y) / 3.f;

			m_Camera->Rotate(deltaX, deltaY);

			SetCursorPos(oldCursor.x, oldCursor.y);
		}
		break;
	}
}

void CTestSceneDX11::UpdateObject(float elapsedTime, void* command)
{
	for (std::shared_ptr<CGameObject>& object : m_Objects)
		object->UpdateObject(elapsedTime);
}

void CTestSceneDX11::Render(void* command)
{
	ID3D11DeviceContext* context = reinterpret_cast<ID3D11DeviceContext*>(command);

	context->RSSetViewports(1, &m_Viewport);
	m_Shader->SetShader(command);

	m_Camera->UpdateCameraBuffer(command);
	m_Camera->SetShaderVariable(command, ST_VS);

	for (std::shared_ptr<CGameObject>& object : m_Objects)
		object->Render(command);
}