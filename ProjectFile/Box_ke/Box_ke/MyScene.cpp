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


// ====================================================================

void CShaderTestSceneDX11::Setup(void* device, void* command)
{
	ID3D11Device* dev = reinterpret_cast<ID3D11Device*>(device);
	tempdev = dev;
	m_Shader = std::make_unique<CPhongLightShader>(dev);

	m_Camera = std::make_unique<CCameraDX11>(device, 60.f, 16.f / 9.f, 0.01f, 500.f);
	m_Camera->SetStartSlot(1);
	m_Camera->SetCameraEYE(0.f, 0.f, -30.f);

	m_Viewport = { 0.f, 0.f, 1920.f, 1080.f, 0.f, 1.f };

	std::shared_ptr<CMesh> mesh0 = std::make_shared<CNormalMeshDX11>(device, 5.f, 5.f, 5.f);
	std::shared_ptr<CMesh> mesh1 = std::make_shared<CNormalMeshDX11>(device, 2.5f, 32U);
	std::shared_ptr<CMesh> mesh2 = std::make_shared<CNormalMeshDX11>(device, 100.f, 100.f);
	PhongMaterialCbuffer material = {
		.diffuseColor = {1.f, 0.f, 0.f, 1.f},
		.specularColor = {1.f, 1.f, 1.f, 1.f},
		.ambientColor = {1.f, 0.f, 0.f, 1.f},
		.emissiveColor = {0.f, 0.f, 0.f, 0.f},
		.shininess = 32.f
	};
	std::shared_ptr<CMaterial> material1 = std::make_shared<CPhongShadingMaterialDX11>(dev, material);
	material = {
		.diffuseColor = {0.5f, 0.5f, 0.5f, 1.f},
		.specularColor = {1.f, 1.f, 1.f, 1.f},
		.ambientColor = {0.5f, 0.5f, 0.5f, 1.f},
		.emissiveColor = {0.f, 0.f, 0.f, 0.f},
		.shininess = 128.f
	};
	std::shared_ptr<CMaterial> material2 = std::make_shared<CPhongShadingMaterialDX11>(dev, material);
	material1->SetStartSlot(2);
	material2->SetStartSlot(2);
	{
		std::shared_ptr<CGameObject> sp = std::make_shared<COrbitObjectDX11>(device);
		m_Objects.push_back(sp);
		auto* p = dynamic_cast<COrbitObjectDX11*>(sp.get());
		p->SetMesh(mesh0);
		p->GetMaterials().push_back(material1);

		auto& c = p->GetChilds();
		std::shared_ptr<CHierarchyGameObjectDX11> cp = std::make_shared<CCircleObjectDX11>(device);
		c.push_back(cp);
		auto& cc = cp->GetChilds();

		cp = std::make_shared<COrbitObjectDX11>(device);
		cp->SetMesh(mesh1);
		cp->GetMaterials().push_back(material1);
		cp->SetPosition(20.f, 0.f, 0.f);
		cc.push_back(cp);

		cp = std::make_shared<CCircleObjectDX11>(device);
		c.push_back(cp);
		dynamic_cast<CCircleObjectDX11*>(cp.get())->Roll();
		auto& cd = cp->GetChilds();

		cp = std::make_shared<COrbitObjectDX11>(device);
		cp->SetMesh(mesh0);
		cp->GetMaterials().push_back(material1);
		cp->SetPosition(10.f, 0.f, 0.f);
		cd.push_back(cp);

		m_Objects.emplace_back(std::make_shared<CGameObjectDX11>(device));
		m_Objects.back()->GetMaterials().push_back(material2);
		m_Objects.back()->SetMesh(mesh2);
		m_Objects.back()->RotateAbsAxis(90.0f);
		m_Objects.back()->SetPosition(0.f, -20.f, 0.f);
	}
}

void CShaderTestSceneDX11::ProcessInput(float elapsedTime)
{
	BYTE keyboard[256];
	BOOL ret = GetKeyboardState(keyboard);

	m_Camera->MakeRightVec();
	if (keyboard['W'] & 0xf0)
		m_Camera->Move(10.0f, elapsedTime);
	if (keyboard['S'] & 0xf0) {
		XMFLOAT3 look = m_Camera->GetLookVec();
		look.x *= -1; look.y *= -1; look.z *= -1;
		m_Camera->Move(10.0f, elapsedTime, look);
	}
	if (keyboard['D'] & 0xf0) {
		m_Camera->Move(10.f, elapsedTime, m_Camera->GetRightVec());
	}
	if (keyboard['A'] & 0xf0) {
		XMFLOAT3 right = m_Camera->GetRightVec();
		right.x *= -1; right.y *= -1; right.z *= -1;
		m_Camera->Move(10.f, elapsedTime, right);
	}
	if (keyboard[VK_SPACE] & 0xf0) {
		m_Camera->Move(10.f, elapsedTime, XMFLOAT3(0.f, 1.f, 0.f));
	}
	if (keyboard[VK_CONTROL] & 0xf0) {
		m_Camera->Move(10.f, elapsedTime, XMFLOAT3(0.f, -1.f, 0.f));
	}
}

void CShaderTestSceneDX11::KeyboardMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

void CShaderTestSceneDX11::MouseMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

void CShaderTestSceneDX11::UpdateObject(float elapsedTime, void* command)
{
	for (std::shared_ptr<CGameObject>& object : m_Objects)
		object->UpdateObject(elapsedTime);
}

void CShaderTestSceneDX11::Render(void* command)
{
	ID3D11DeviceContext* context = reinterpret_cast<ID3D11DeviceContext*>(command);

	context->RSSetViewports(1, &m_Viewport);
	m_Shader->SetShader(command);

	m_Camera->UpdateCameraBuffer(command);
	m_Camera->SetShaderVariable(command, ST_VS);
	m_Camera->SetShaderVariable(command, ST_PS);

	for (std::shared_ptr<CGameObject>& object : m_Objects)
		object->Render(command);
}