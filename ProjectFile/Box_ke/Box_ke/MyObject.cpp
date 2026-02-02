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

	if (!m_Animplayer->GetPlaying()) {
		if (m_Animplayer->GetAnimationName() == "RollTurn_On") {
			m_Animplayer->Play("AttackShake_Hero", true);
		}
	}

	KeyframeDesc pose = m_Animplayer->GetCurrentPose();


	SetPosition(pose.Position.x, pose.Position.y, pose.Position.z);
	RotateLocalAxis(pose.RotationQuat);

	for (int i = 0; i < m_Childs.size(); ++i)
		m_Childs[i]->UpdateObject(elapsedTime);
}

void CHeroObjectDX11::Initialize()
{
	m_Animplayer = std::make_shared<CAnimPlayer>();
	auto RollTurn_On = std::make_shared<CAnimation>();
	RollTurn_On->AddKey(0.f, { 0.f + GetPosition().x, 0.f + GetPosition().y, 0.f + GetPosition().z }, { 0,1,0 }, 0.0f, { 1,1,1 });
	RollTurn_On->AddKey(0.2f, { 2.f + GetPosition().x, 0.f + GetPosition().y, 0.f + GetPosition().z }, { 0,1,0 }, -90.0f, { 1,1,1 });
	RollTurn_On->AddKey(0.3f, { 3.f + GetPosition().x, 0.f + GetPosition().y, 0.f + GetPosition().z }, { 0,1,0 }, -180.0f, { 1,1,1 });
	m_Animplayer->AddAnimation("RollTurn_On", RollTurn_On);

	auto RollTurn_Off = std::make_shared<CAnimation>();
	RollTurn_Off->AddKey(0.0f, { 3.f + GetPosition().x, 0.f + GetPosition().y, 0.f + GetPosition().z }, { 0,1,0 }, 180.0f, { 1,1,1 });
	RollTurn_Off->AddKey(0.3f, { 0.f + GetPosition().x, 0.f + GetPosition().y, 0.f + GetPosition().z }, { 0,1,0 }, 0.0f, { 1,1,1 });
	m_Animplayer->AddAnimation("RollTurn_Off", RollTurn_Off);

	auto AttackShake_Hero = std::make_shared<CAnimation>();
	AttackShake_Hero->AddKey(0.f, { 3.f,0,0 }, { 0,1,0 }, 180.0f, { 1,1,1 });
	AttackShake_Hero->AddKey(0.05f, { 3.f,0,0 }, { 0,1,0 }, 183.f, { 1,1,1 });
	AttackShake_Hero->AddKey(0.1f, { 3.f,0,0 }, { 0,1,0 }, 177.f, { 1,1,1 });
	AttackShake_Hero->AddKey(0.15f, { 3.f,0,0 }, { 0,1,0 }, 180.0f, { 1,1,1 });
	m_Animplayer->AddAnimation("AttackShake_Hero", AttackShake_Hero);

	auto RollRight = std::make_shared<CAnimation>();
	RollRight->AddKey(0.0f, { 0.f + GetPosition().x, 0.f + GetPosition().y, 0.f + GetPosition().z }, { 0,0,1 }, 0.0f, { 1,1,1 });
	RollRight->AddKey(0.2f, { 0.f + GetPosition().x, 2.f + GetPosition().y, 0.f + GetPosition().z }, { 0,0,1 }, -90.f, { 1,1,1 });
	RollRight->AddKey(0.5f, { 0.f + GetPosition().x, 0.f + GetPosition().y, 0.f + GetPosition().z }, { 0,0,1 }, -180.f, { 1,1,1 });
	RollRight->AddKey(0.7f, { 0.f + GetPosition().x, 1.f + GetPosition().y, 0.f + GetPosition().z }, { 0,0,1 }, -250.f, { 1,1,1 });
	RollRight->AddKey(1.f, { 0.f + GetPosition().x, 0.f + GetPosition().y, 0.f + GetPosition().z }, { 0,0,1 }, 0.0f, { 1,1,1 });

	m_Animplayer->AddAnimation("RollRight", RollRight);

	auto RollLeft = std::make_shared<CAnimation>();
	RollLeft->AddKey(0.0f, { 0.f + GetPosition().x, 0.f + GetPosition().y, 0.f + GetPosition().z }, { 0,0,1 }, 0.0f, { 1,1,1 });
	RollLeft->AddKey(0.2f, { 0.f + GetPosition().x, 2.f + GetPosition().y, 0.f + GetPosition().z }, { 0,0,1 }, 90.f, { 1,1,1 });
	RollLeft->AddKey(0.5f, { 0.f + GetPosition().x, 0.f + GetPosition().y, 0.f + GetPosition().z }, { 0,0,1 }, 180.f, { 1,1,1 });
	RollLeft->AddKey(0.7f, { 0.f + GetPosition().x, 2.f + GetPosition().y, 0.f + GetPosition().z }, { 0,0,1 }, 250.f, { 1,1,1 });
	RollLeft->AddKey(1.f, { 0.f + GetPosition().x, 0.f + GetPosition().y, 0.f + GetPosition().z }, { 0,0,1 }, 0.0f, { 1,1,1 });

	m_Animplayer->AddAnimation("RollLeft", RollLeft);



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
	m_GunAnimplayer = std::make_shared<CAnimPlayer>();
	m_GunAnimplayer->SetCurrentPose(GetPosition().x, GetPosition().y, GetPosition().z);
	m_GunAnimplayer->SetCurrentRotationQuat(0, 0, -45.f);

	auto GunLiftRight_On = std::make_shared<CAnimation>();
	GunLiftRight_On->AddKey(0.f, { 0.f + GetPosition().x, 0.f + GetPosition().y, 0.f + GetPosition().z }, { 1,0,0 }, 0.0f, {1,1,1});
	GunLiftRight_On->AddKey(0.3f, { 3.f + GetPosition().x , 0.f + GetPosition().y, 0.f + GetPosition().z }, { 1,0,0 }, -90.f, { 1,1,1 });

	m_GunAnimplayer->AddAnimation("GunLift_Right_On", GunLiftRight_On);


	auto GunLiftRight_Off = std::make_shared<CAnimation>();
	GunLiftRight_Off->AddKey(0.f, { 3.f + GetPosition().x , 0.f + GetPosition().y, 0.f + GetPosition().z }, { 1,0,0 }, -90.f, { 1,1,1 });
	GunLiftRight_Off->AddKey(0.3f, { 0.f + GetPosition().x, 0.f + GetPosition().y, 0.f + GetPosition().z }, { 0,0,1 }, -45.f, { 1,1,1 });
	m_GunAnimplayer->AddAnimation("GunLift_Right_Off", GunLiftRight_Off);


	auto AttackShake_Gun = std::make_shared<CAnimation>();
	AttackShake_Gun->AddKey(0.f, { 3.f + GetPosition().x, -1.f + GetPosition().y, 0.f + GetPosition().z }, { 1,0,0 }, 90.f, { 1,1,1 });
	AttackShake_Gun->AddKey(0.05f, { 3.f + GetPosition().x, 0.f + GetPosition().y, 0.f + GetPosition().z }, { 1,0,0 }, 90.f, { 1,1,1 });
	AttackShake_Gun->AddKey(0.1f, { 3.f + GetPosition().x, 0.3f + GetPosition().y, 0.f + GetPosition().z }, { 1,0,0 }, 90.f, { 1,1,1 });
	AttackShake_Gun->AddKey(0.15f, { 3.f + GetPosition().x, -1.f + GetPosition().y, 0.f + GetPosition().z }, { 1,0,0 }, 90.f, { 1,1,1 });
	m_GunAnimplayer->AddAnimation("AttackShake_Gun", AttackShake_Gun);


}
void CGunObjectDX11::UpdateObject(float elapsedTime)
{
	if (m_Hero)
	{
		m_GunAnimplayer->Update(elapsedTime);
		if (static_cast<CHeroObjectDX11*>(m_Hero.get())->GetAnimPlayer()->GetAnimationName() == "RollTurn_On" 
			&& m_GunAnimplayer->GetAnimationName() != "GunLift_Right_On") {
			m_GunAnimplayer->Play("GunLift_Right_On", false);
		}

		if (static_cast<CHeroObjectDX11*>(m_Hero.get())->GetAnimPlayer()->GetAnimationName() == "RollTurn_Off"
			&& m_GunAnimplayer->GetAnimationName() != "GunLift_Right_Off") {
			m_GunAnimplayer->Play("GunLift_Right_Off", false);
		}
		
		if (static_cast<CHeroObjectDX11*>(m_Hero.get())->GetAnimPlayer()->GetAnimationName() == "AttackShake_Hero"
			&&m_GunAnimplayer->GetAnimationName() != "AttackShake_Gun") {
			m_GunAnimplayer->Play("AttackShake_Gun", true);

		}
		

		KeyframeDesc pose = m_GunAnimplayer->GetCurrentPose();


		SetPosition(pose.Position.x, pose.Position.y, pose.Position.z);
		RotateLocalAxis(pose.RotationQuat);
	}





}
void CGunObjectDX11::AimAtMouse()
{



}




CBoxObjectDX11::CBoxObjectDX11(void* command) : CHierarchyGameObjectDX11(command)
{
}

void CBoxObjectDX11::UpdateObject(float elapsedTime)
{
}

void CBoxObjectDX11::Roll()
{
}
