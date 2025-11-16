#include "TestScene.h"

void CHeroScene::Setup(void* device, void* command)
{
	ID3D11Device* dev = reinterpret_cast<ID3D11Device*>(device);
	tempdev = dev;
	m_Shader = std::make_unique<CPhongLightShaderDX11>(dev);

	m_Camera = std::make_unique<CCameraDX11>(device, 60.f, 16.f / 9.f, 0.01f, 500.f);
	m_Camera->SetStartSlot(1);
	m_Camera->SetCameraEYE(0.f, 5.f, -20.f);
	

	m_Viewport = { 0.f, 0.f, 1920.f, 1080.f, 0.f, 1.f };
	// 메인 몸통,  총 하나
	std::shared_ptr<CMesh> BodyMesh = std::make_shared<CNormalMeshDX11>(device, 4.f, 8.f, 4.f);
	std::shared_ptr<CMesh> GunMesh = std::make_shared<CNormalMeshDX11>(device, 1.f, 4.f, 1.f);
	std::shared_ptr<CMesh> LaserMesh = std::make_shared<CNormalMeshDX11>(device, 0.1f, 100.f, 0.1f);
	
	std::shared_ptr<CMesh> FloorMesh = std::make_shared<CNormalMeshDX11>(device, 1000.f, 1000.f);

	PhongMaterialCbuffer material = {
		.diffuseColor = {0.3f, 0.3f, 0.3f, 0.4f},
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

	std::shared_ptr<CMaterial> GunMaterial = std::make_shared<CPhongShadingMaterialDX11>(dev, material);


	material = {
		.diffuseColor = {0.5f, 0.5f, 0.5f, 1.f},
		.specularColor = {1.f, 1.f, 1.f, 1.f},
		.ambientColor = {0.5f, 0.5f, 0.5f, 1.f},
		.emissiveColor = {0.f, 0.f, 0.f, 0.f},
		.shininess = 128.f
	};

	std::shared_ptr<CMaterial> FloorMaterial = std::make_shared<CPhongShadingMaterialDX11>(dev, material);
	
	BodyMaterial->SetStartSlot(2);
	GunMaterial->SetStartSlot(2);

	FloorMaterial->SetStartSlot(2);


	{
		std::shared_ptr<CGameObject> hero = std::make_shared<CHeroObjectDX11>(device);
		m_Objects.push_back(hero);


		auto* p_Hero = static_cast<CHeroObjectDX11*>(hero.get());
		auto& c_Hero = p_Hero->GetChilds();
		m_Hero = hero;
		
		p_Hero->Initialize();

		// 몸체
		std::shared_ptr<CHierarchyGameObjectDX11> MainBody = std::make_shared<CBodyObjectDX11>(device);
		MainBody->SetMesh(BodyMesh);
		MainBody->GetMaterials().push_back(BodyMaterial);
		MainBody->SetPosition(0.f, 0.f, 0.f);
		MainBody->RotateLocalAxis(0.f, 180.f, 0.f);
		c_Hero.push_back(MainBody);

		// 총
		std::shared_ptr<CHierarchyGameObjectDX11> Gun = std::make_shared<CGunObjectDX11>(device);

		auto* p_MainBody = static_cast<CBodyObjectDX11*>(MainBody.get());
		auto& c_MainBody = p_MainBody->GetChilds();
		Gun->SetMesh(GunMesh);
		Gun->GetMaterials().push_back(GunMaterial);
		Gun->SetPosition(0.f, 0.f, 3.f);
		
		static_cast<CGunObjectDX11*>(Gun.get())->Initialize();
		static_cast<CGunObjectDX11*>(Gun.get())->SetHero(m_Hero);

		c_MainBody.push_back(Gun);


		// 레이저 
		std::shared_ptr<CHierarchyGameObjectDX11> Laser = std::make_shared<CBoxObjectDX11>(device);

		auto* p_GunBody = static_cast<CBodyObjectDX11*>(Gun.get());
		auto& c_GunBody = p_GunBody->GetChilds();
		Laser->SetMesh(LaserMesh);
		Laser->GetMaterials().push_back(GunMaterial);
		Laser->SetPosition(0.f, 50.f, 0.f);

		c_GunBody.push_back(Laser);



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
	if (keyboard['E'] & 0xf0) {
		if (m_Hero) {
			auto AnimName = static_cast<CHeroObjectDX11*>(m_Hero.get())->GetAnimPlayer()->GetAnimationName();
			static_cast<CHeroObjectDX11*>(m_Hero.get())->GetAnimPlayer()->Play("RollRight", false);
		}
	}
	if (keyboard['Q'] & 0xf0) {
		if (m_Hero) {
			auto AnimName = static_cast<CHeroObjectDX11*>(m_Hero.get())->GetAnimPlayer()->GetAnimationName();
			static_cast<CHeroObjectDX11*>(m_Hero.get())->GetAnimPlayer()->Play("RollLeft", false);
		}
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
				if (m_Hero) {
				auto AnimName = static_cast<CHeroObjectDX11*>(m_Hero.get())->GetAnimPlayer()->GetAnimationName();
				static_cast<CHeroObjectDX11*>(m_Hero.get())->GetAnimPlayer()->Play("RollTurn_On", false);
				}

		}
		break;
	case WM_LBUTTONUP:
		if (click) {
			click = false;
			if (m_Hero) {
				auto AnimName = static_cast<CHeroObjectDX11*>(m_Hero.get())->GetAnimPlayer()->GetAnimationName();
				static_cast<CHeroObjectDX11*>(m_Hero.get())->GetAnimPlayer()->Play("RollTurn_Off", false);
			}
		}
		break;
	case WM_MOUSEMOVE:
	/*	if (click) {
			POINT curPos{};
			GetCursorPos(&curPos);

			float deltaX = static_cast<float>(curPos.x - oldCursor.x) / 3.f;
			float deltaY = static_cast<float>(curPos.y - oldCursor.y) / 3.f;

			m_Camera->Rotate(deltaX, deltaY);

			SetCursorPos(oldCursor.x, oldCursor.y);
		}*/
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