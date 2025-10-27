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
	m_Shader = std::make_unique<CPhongLightShaderDX11>(dev);

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

// ======================================================================

CDeferredRenderSceneDX11::CDeferredRenderSceneDX11(ComPtr<ID3D11Device4> device, ComPtr<IDXGISwapChain4> swapChain,
	ComPtr<ID3D11RenderTargetView> rtv, ComPtr<ID3D11DepthStencilView> dsv,
	UINT& clientWidth, UINT& clientHeight)
	: m_Device{ device }, m_SwapChain{ swapChain }, m_MainRTV{ rtv }, m_MainDSV{ dsv }, m_ClientWidth{ clientWidth }, m_ClientHeight{ clientHeight }
{
	CreateTargets();
	BuildObjects();
}

void CDeferredRenderSceneDX11::ResizeTargets()
{

}

void CDeferredRenderSceneDX11::CreateTargets()
{
	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = m_ClientWidth;
	desc.Height = m_ClientHeight;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	
	HRESULT hResult;

	for (int i = 0; i < 4; ++i) {
		ComPtr<ID3D11Texture2D> texture{};
		ComPtr<ID3D11ShaderResourceView> srv{};
		ComPtr<ID3D11RenderTargetView> rtv{};
		hResult = m_Device->CreateTexture2D(&desc, nullptr, texture.GetAddressOf());
		if (FAILED(hResult)) {
			MessageBoxA(0, "CDeferredRenderSceneDX11::CreateTargets()\nCreateTexture2D Failed", "Fatal Error", MB_OK);
			return;
		}
		hResult = m_Device->CreateRenderTargetView(texture.Get(), nullptr, rtv.GetAddressOf());
		if (FAILED(hResult)) {
			MessageBoxA(0, "CDeferredRenderSceneDX11::CreateTargets()\nCreateRenderTargetView Failed", "Fatal Error", MB_OK);
			return;
		}
		hResult = m_Device->CreateShaderResourceView(texture.Get(), nullptr, srv.GetAddressOf());
		if (FAILED(hResult)) {
			MessageBoxA(0, "CDeferredRenderSceneDX11::CreateTargets()\nCreateShaderResourceView Failed", "Fatal Error", MB_OK);
			return;
		}

		m_RTVs.push_back(rtv);
		m_SRVs.push_back(srv);
	}


	// OutputTexture
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_Device->CreateTexture2D(&desc, nullptr, m_OutputTexture.GetAddressOf());
	m_Device->CreateShaderResourceView(m_OutputTexture.Get(), nullptr, m_OutputSRV.GetAddressOf());
	m_Device->CreateRenderTargetView(m_OutputTexture.Get(), nullptr, m_OutputRTV.GetAddressOf());
}

void CDeferredRenderSceneDX11::BuildObjects()
{
	m_MRTShader = std::make_shared<CDeferredRenderingOnePathShaderDX11>(m_Device.Get());
	m_RenderShader = std::make_shared<CDeferredRenderingTwoPathShaderDX11>(m_Device.Get());
	m_TextureRenderShader = std::make_shared<CFullScreenTextrueRenderShaderDX11>(m_Device.Get());

	m_Camera = std::make_unique<CCameraDX11>(m_Device.Get(), 60.f, 16.f / 9.f, 0.01f, 500.f);
	m_Camera->SetStartSlot(1);
	m_Camera->SetCameraEYE(0.f, 0.f, -30.f);

	m_Viewport = { 0.f, 0.f, 1920.f, 1080.f, 0.f, 1.f };

	std::shared_ptr<CMesh> mesh0 = std::make_shared<CNormalMeshDX11>(m_Device.Get(), 5.f, 5.f, 5.f);
	std::shared_ptr<CMesh> mesh1 = std::make_shared<CNormalMeshDX11>(m_Device.Get(), 2.5f, 32U);
	std::shared_ptr<CMesh> mesh2 = std::make_shared<CNormalMeshDX11>(m_Device.Get(), 100.f, 100.f);
	PhongMaterialCbuffer material = {
		.diffuseColor = {1.f, 1.f, 0.f, 1.f},
		.specularColor = {1.f, 1.f, 1.f, 1.f},
		.ambientColor = {1.f, 1.f, 0.f, 1.f},
		.emissiveColor = {1.f, 1.f, 0.f, 0.f},
		.shininess = 32.f
	};
	std::shared_ptr<CMaterial> material1 = std::make_shared<CPhongShadingMaterialDX11>(m_Device.Get(), material);
	material = {
		.diffuseColor = {0.5f, 0.5f, 0.5f, 1.f},
		.specularColor = {1.f, 1.f, 1.f, 1.f},
		.ambientColor = {0.5f, 0.5f, 0.5f, 1.f},
		.emissiveColor = {0.f, 0.f, 0.f, 0.f},
		.shininess = 128.f
	};
	std::shared_ptr<CMaterial> material2 = std::make_shared<CPhongShadingMaterialDX11>(m_Device.Get(), material);
	material1->SetStartSlot(2);
	material2->SetStartSlot(2);
	{
		std::shared_ptr<CGameObject> sp = std::make_shared<COrbitObjectDX11>(m_Device.Get());
		m_Objects.push_back(sp);
		auto* p = dynamic_cast<COrbitObjectDX11*>(sp.get());
		p->SetMesh(mesh0);
		p->GetMaterials().push_back(material1);

		auto& c = p->GetChilds();
		std::shared_ptr<CHierarchyGameObjectDX11> cp = std::make_shared<CCircleObjectDX11>(m_Device.Get());
		c.push_back(cp);
		auto& cc = cp->GetChilds();

		cp = std::make_shared<COrbitObjectDX11>(m_Device.Get());
		cp->SetMesh(mesh1);
		cp->GetMaterials().push_back(material1);
		cp->SetPosition(20.f, 0.f, 0.f);
		cc.push_back(cp);

		cp = std::make_shared<CCircleObjectDX11>(m_Device.Get());
		c.push_back(cp);
		dynamic_cast<CCircleObjectDX11*>(cp.get())->Roll();
		auto& cd = cp->GetChilds();

		cp = std::make_shared<COrbitObjectDX11>(m_Device.Get());
		cp->SetMesh(mesh0);
		cp->GetMaterials().push_back(material1);
		cp->SetPosition(10.f, 0.f, 0.f);
		cd.push_back(cp);

		m_Objects.emplace_back(std::make_shared<CGameObjectDX11>(m_Device.Get()));
		m_Objects.back()->GetMaterials().push_back(material2);
		m_Objects.back()->SetMesh(mesh2);
		m_Objects.back()->RotateAbsAxis(90.0f);
		m_Objects.back()->SetPosition(0.f, -20.f, 0.f);
	}

	m_BloomProcessor = std::make_shared<CBloomProcessorDX11>(m_ClientWidth, m_ClientHeight, m_Device.Get(), m_TextureRenderShader);
}

void CDeferredRenderSceneDX11::ProcessInput(float elapsedTime)
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

void CDeferredRenderSceneDX11::KeyboardMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_KEYDOWN:
		switch (wParam) {
		case '1':
			m_MRTShader->ShaderReCompile(m_Device.Get());
			break;
		case '2':
			m_RenderShader->ShaderReCompile(m_Device.Get());
			break;
		case '3':
			break;
		case 'B':
			m_OnBloom = !m_OnBloom;
			break;
		}
		break;
	}
}

void CDeferredRenderSceneDX11::MouseMessageProcessing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

void CDeferredRenderSceneDX11::UpdateObject(float elapsedTime, void* command)
{
	/*for (std::shared_ptr<CGameObject>& object : m_Objects)
		object->UpdateObject(elapsedTime);*/
}

void CDeferredRenderSceneDX11::PreRender(void* command)
{
	ID3D11DeviceContext* context = reinterpret_cast<ID3D11DeviceContext*>(command);
	float clearColor[] = { 0.f, 0.f, 0.f, 1.f };
	for (int i = 0; i < m_RTVs.size(); ++i)
		context->ClearRenderTargetView(m_RTVs[i].Get(), clearColor);
	ID3D11RenderTargetView* views[] = {
		m_RTVs[0].Get(), m_RTVs[1].Get(), m_RTVs[2].Get(), m_RTVs[3].Get()
	};
	context->OMSetRenderTargets(4, views, m_MainDSV.Get());
}

void CDeferredRenderSceneDX11::Render(void* command)
{
	ID3D11DeviceContext* context = reinterpret_cast<ID3D11DeviceContext*>(command);
	context->RSSetViewports(1, &m_Viewport);
	m_Camera->UpdateCameraBuffer(command);
	m_Camera->SetShaderVariable(command, ST_VS);
	m_Camera->SetShaderVariable(command, ST_PS);

	OnePathRender(context);
	TwoPathRender(context);
}

void CDeferredRenderSceneDX11::PostRender(void* command)
{
	ID3D11DeviceContext* context = reinterpret_cast<ID3D11DeviceContext*>(command);
	if (m_OnBloom) {
		m_BloomProcessor->Process(context, m_OutputTexture.Get(), m_OutputTexture.Get());
	}

	m_TextureRenderShader->SetShader(command);
	context->OMSetRenderTargets(1, m_MainRTV.GetAddressOf(), nullptr);
	context->RSSetViewports(1, &m_Viewport);
	context->PSSetShaderResources(0, 1, m_OutputSRV.GetAddressOf());

	context->DrawInstanced(6, 1, 0, 0);
}

void CDeferredRenderSceneDX11::OnePathRender(ID3D11DeviceContext* context)
{
	m_MRTShader->SetShader(context);

	for (std::shared_ptr<CGameObject>& object : m_Objects)
		object->Render(context);
}

void CDeferredRenderSceneDX11::TwoPathRender(ID3D11DeviceContext* context)
{
	m_RenderShader->SetShader(context);

	context->OMSetRenderTargets(1, m_OutputRTV.GetAddressOf(), nullptr);
	ID3D11ShaderResourceView* views[4] = {
		m_SRVs[0].Get(), m_SRVs[1].Get(), m_SRVs[2].Get(), m_SRVs[3].Get()
	};
	context->PSSetShaderResources(0, 4, views);
	m_Camera->SetShaderVariable(context, ST_PS);
	context->DrawInstanced(6, 1, 0, 0);
}