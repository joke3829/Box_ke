#include "MyObject.h"

CCircleObjectDX11::CCircleObjectDX11(void* command)
	: CHierarchyGameObjectDX11(command)
{

}

void CCircleObjectDX11::UpdateObject(float elapsedTime)
{
	RotateLocalAxis(0.f, 30.f * elapsedTime, 0.f);
	for (int i = 0; i < m_Childs.size(); ++i)
		m_Childs[i]->UpdateObject(elapsedTime);
}

void CCircleObjectDX11::Roll()
{
	RotateLocalAxis(0.f, 0.f, 90.f);
}



COrbitObjectDX11::COrbitObjectDX11(void* device)
	: CHierarchyGameObjectDX11(device)
{

}

void COrbitObjectDX11::UpdateObject(float elapsedTime)
{
	float rotAngle = 30.f * elapsedTime;
	RotateLocalAxis(rotAngle, rotAngle, rotAngle);
	for (int i = 0; i < m_Childs.size(); ++i)
		m_Childs[i]->UpdateObject(elapsedTime);
}

//---------------------------------------------------------------------------------------------------------------------------



CHeroObjectDX11::CHeroObjectDX11(void* device) : CHierarchyGameObjectDX11(device)
{

}

void CHeroObjectDX11::UpdateObject(float elapsedTime)
{

	m_Animplayer->Update(elapsedTime);

	KeyframeDesc pose = m_Animplayer->GetCurrentPose();

	RotateLocalAxis(pose.RotationQuat);

	for (int i = 0; i < m_Childs.size(); ++i)
		m_Childs[i]->UpdateObject(elapsedTime);
}

void CHeroObjectDX11::Initialize()
{
	m_Animplayer = std::make_shared<CAnimPlayer>();
	auto RollRight_On = std::make_shared<CAnimation>();
	RollRight_On->AddKey(0.f, { 0,0,0 }, { 0,0,0,1 }, { 1,1,1 });
	RollRight_On->AddKey(0.2f, { 0,0,0 }, { 0,-1,0,1 }, { 1,1,1 });
	RollRight_On->AddKey(0.5f, { 0,0,0 }, { 0,-2,0,1 }, { 1,1,1 });
	m_Animplayer->AddAnimation("RollRight_On", RollRight_On);

	auto RollRight_Off = std::make_shared<CAnimation>();
	RollRight_Off->AddKey(0.f, { 0,0,0 }, { 0,-1,0,1 }, { 1,1,1 });
	RollRight_Off->AddKey(0.2f, { 0,0,0 }, { 0,0,0,1 }, { 1,1,1 });
	m_Animplayer->AddAnimation("RollRight_Off", RollRight_Off);

	auto RollLeft_On = std::make_shared<CAnimation>();
	RollLeft_On->AddKey(0.f, { 0,0,0 }, { 0,0,0,1 }, { 1,1,1 });
	RollLeft_On->AddKey(0.2f, { 0,0,0 }, { 0,1,0,1 }, { 1,1,1 });
	m_Animplayer->AddAnimation("RollLeft_On", RollLeft_On);

	auto RollLeft_Off = std::make_shared<CAnimation>();
	RollLeft_Off->AddKey(0.f, { 0,0,0 }, { 0,-1,0,1 }, { 1,1,1 });
	RollLeft_Off->AddKey(0.2f, { 0,0,0 }, { 0,0,0,1 }, { 1,1,1 });
	m_Animplayer->AddAnimation("RollLeft_Off", RollLeft_Off);

}






//---------------------------------------------------------------------------------------------------------------------------


CBodyObjectDX11::CBodyObjectDX11(void* device) : CHeroObjectDX11(device)
{
}

void CBodyObjectDX11::Initialize()
{
	
}


void CBodyObjectDX11::UpdateObject(float elapsedTime)
{

	for (int i = 0; i < m_Childs.size(); ++i)
		m_Childs[i]->UpdateObject(elapsedTime);
}




//---------------------------------------------------------------------------------------------------------------------------

CGunObjectDX11::CGunObjectDX11(void* device) : CHeroObjectDX11(device)
{
}

void CGunObjectDX11::Initialize()
{
	
}
void CGunObjectDX11::UpdateObject(float elapsedTime)
{



}



