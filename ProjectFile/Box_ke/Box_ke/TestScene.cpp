#include "TestScene.h"

void CHeroScene::Setup(void* device, void* command)
{
	ID3D11Device* dev = reinterpret_cast<ID3D11Device*>(device);
	tempdev = dev;
	m_Shader = std::make_unique<CPhongLightShaderDX11>(dev);

	m_Camera = std::make_unique<CCameraDX11>(device, 60.f, 16.f / 9.f, 0.01f, 500.f);
	m_Camera->SetStartSlot(1);
	m_Camera->SetCameraEYE(0.f, 10.f, -30.f);

	m_Viewport = { 0.f, 0.f, 1920.f, 1080.f, 0.f, 1.f };
	// 메인 몸통, 서브 몸통 2개 (왼, 오), 메인 팔 2개 (왼, 오), 서브 팔 2개 (왼, 오)
	std::shared_ptr<CMesh> BodyMesh = std::make_shared<CNormalMeshDX11>(device, 2.5f, 15.f, 2.5f);
	std::shared_ptr<CMesh> ArmMesh = std::make_shared<CNormalMeshDX11>(device, 2.5f, 15.f, 2.5f);
	
	std::shared_ptr<CMesh> FloorMesh = std::make_shared<CNormalMeshDX11>(device, 100.f, 100.f);

	PhongMaterialCbuffer material = {
		.diffuseColor = {0.5f, 0.f, 0.f, 1.f},
		.specularColor = {1.f, 1.f, 1.f, 1.f},
		.ambientColor = {1.f, 0.f, 0.f, 1.f},
		.emissiveColor = {0.f, 0.f, 0.f, 0.f},
		.shininess = 32.f / 2
	};
	std::shared_ptr<CMaterial> BodyMaterial = std::make_shared<CPhongShadingMaterialDX11>(dev, material);

	material = {
		.diffuseColor = {0.5f, 0.f, 0.5f, 1.f},
		.specularColor = {1.f, 1.f, 1.f, 1.f},
		.ambientColor = {1.f, 0.f, 0.f, 1.f},
		.emissiveColor = {0.f, 0.f, 0.f, 0.f},
		.shininess = 32.f /2 
	};

	std::shared_ptr<CMaterial> ArmMaterial = std::make_shared<CPhongShadingMaterialDX11>(dev, material);

	material = {
		.diffuseColor = {0.5f, 0.5f, 0.5f, 1.f},
		.specularColor = {1.f, 1.f, 1.f, 1.f},
		.ambientColor = {0.5f, 0.5f, 0.5f, 1.f},
		.emissiveColor = {0.f, 0.f, 0.f, 0.f},
		.shininess = 128.f
	};

	std::shared_ptr<CMaterial> FloorMaterial = std::make_shared<CPhongShadingMaterialDX11>(dev, material);
	
	BodyMaterial->SetStartSlot(2);
	ArmMaterial->SetStartSlot(2);
	FloorMaterial->SetStartSlot(2);


	{
		std::shared_ptr<CGameObject> hero = std::make_shared<CHeroObjectDX11>(device);
		m_Objects.push_back(hero);
		auto* p_Hero = static_cast<CHeroObjectDX11*>(hero.get());
		auto& c_Hero = p_Hero->GetChilds();
		m_Hero = hero;

		// 왼쪽 몸체
		std::shared_ptr<CHierarchyGameObjectDX11> LeftBody = std::make_shared<CBodyObjectDX11>(device);
		LeftBody->SetMesh(BodyMesh);
		LeftBody->GetMaterials().push_back(BodyMaterial);
		LeftBody->SetPosition(-1.25f, 0.f, 0.f);
		c_Hero.push_back(LeftBody);

		// 오른쪽 몸체
		std::shared_ptr<CHierarchyGameObjectDX11> RightBody = std::make_shared<CBodyObjectDX11>(device);
		RightBody->SetMesh(BodyMesh);
		RightBody->GetMaterials().push_back(BodyMaterial);
		RightBody->SetPosition(1.25f, 0.f, 0.f);
		c_Hero.push_back(RightBody);

		// 왼쪽 팔
		std::shared_ptr<CHierarchyGameObjectDX11> LeftArm = std::make_shared<CArmObjectDX11>(device);

		auto* p_LeftBody = static_cast<CBodyObjectDX11*>(LeftBody.get());
		auto& c_LeftBody = p_LeftBody->GetChilds();

		LeftArm->SetMesh(ArmMesh);
		LeftArm->GetMaterials().push_back(ArmMaterial);
		LeftArm->SetPosition(-2.5f, 0.f, 0.f);
		c_LeftBody.push_back(LeftArm);

		// 오른쪽 팔

		std::shared_ptr<CHierarchyGameObjectDX11> RightArm = std::make_shared<CArmObjectDX11>(device);


		auto* p_RightBody = static_cast<CBodyObjectDX11*>(RightBody.get());
		auto& c_RightBody = p_RightBody->GetChilds();

		RightArm->SetMesh(ArmMesh);
		RightArm->GetMaterials().push_back(ArmMaterial);
		RightArm->SetPosition(2.5f, 0.f, 0.f);
		c_RightBody.push_back(RightArm);

		auto* p_RightArm = static_cast<CArmObjectDX11*>(RightArm.get());
		auto* p_LeftArm = static_cast<CArmObjectDX11*>(LeftArm.get());



		m_Objects.emplace_back(std::make_shared<CGameObjectDX11>(device));
		m_Objects.back()->GetMaterials().push_back(FloorMaterial);
		m_Objects.back()->SetMesh(FloorMesh);
		m_Objects.back()->RotateAbsAxis(90.0f);
		m_Objects.back()->SetPosition(0.f, -10.f, 0.f);
	}
}

void CHeroScene::ProcessInput(float elapsedTime)
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

void CHeroScene::KeyboardMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_KEYDOWN:
		switch (wParam) {
		case '1':
			m_Shader->ShaderReCompile(tempdev);
			break;
		case '2':
			if (m_Hero) {
				//static_cast<CHeroObjectDX11*>(m_Hero.get())->SetRightRunningAnim(true);
			}
			break;
		}
		break;
	}
}

void CHeroScene::MouseMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

void CHeroScene::UpdateObject(float elapsedTime, void* command)
{
	for (std::shared_ptr<CGameObject>& object : m_Objects)
		object->UpdateObject(elapsedTime);
}

void CHeroScene::Render(void* command)
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