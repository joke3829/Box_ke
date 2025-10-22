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
	UpdateAnim(elapsedTime);


	for (int i = 0; i < m_Childs.size(); ++i)
		m_Childs[i]->UpdateObject(elapsedTime);
}

void CHeroObjectDX11::UpdateAnim(float elapsedTime)
{

	ResetWorldMat();



	if (bPlayingRightRun) {
		
	}
	if (bPlayingLeftRun) {
	
	}
}




void CHeroObjectDX11::SetRightRunningAnim(bool bSet)
{
	bPlayingRightRun = bSet;
	for (int i = 0; i < m_Childs.size(); ++i)
		static_cast<CBodyObjectDX11*>(m_Childs[i].get())->SetRightRunningAnim(bSet);
}

void CHeroObjectDX11::SetLeftRunningAnim(bool bSet)
{
	bPlayingLeftRun = bSet;
	for (int i = 0; i < m_Childs.size(); ++i)
		static_cast<CBodyObjectDX11*>(m_Childs[i].get())->SetLeftRunningAnim(bSet);
}



//---------------------------------------------------------------------------------------------------------------------------


CBodyObjectDX11::CBodyObjectDX11(void* device) : CHierarchyGameObjectDX11(device)
{
}

void CBodyObjectDX11::UpdateObject(float elapsedTime)
{
	UpdateAnim(elapsedTime);

	for (int i = 0; i < m_Childs.size(); ++i)
		m_Childs[i]->UpdateObject(elapsedTime);
}

void CBodyObjectDX11::UpdateAnim(float elapsedTime)
{
	if (bPlayingRightRun) {
		UpdateReadyTurnAnim(elapsedTime);
	}
	if (bPlayingLeftRun) {
		UpdateReadyTurnAnim(elapsedTime);
	}
}


void CBodyObjectDX11::UpdateReadyTurnAnim(float elapsedTime)
{
	
	float rotAngle = 70.f * elapsedTime;

	if (m_TransformReadyTime < 0) {
		RotateLocalAxis(rotAngle, 0.f, 0.f);

		m_Rotate += rotAngle;
		if (m_Rotate > 360)
			m_Rotate = 0;

		if (m_Rotate > m_TransformAngle - 1) {
			bPlayingRightRun = false;
			bPlayingLeftRun = false;
			m_TransformReadyTime = m_TransformPrevReadyTime;
		}
		static_cast<CArmObjectDX11*>(m_Childs[0].get())->SetTransformBodyAngleAnim(rotAngle);
	}
	
	--m_TransformReadyTime;


	
}
void CBodyObjectDX11::SetRightRunningAnim(bool bSet)
{
	bPlayingRightRun = bSet;
	for (int i = 0; i < m_Childs.size(); ++i)
		static_cast<CArmObjectDX11*>(m_Childs[i].get())->SetRightRunningAnim(bSet);
}

void CBodyObjectDX11::SetLeftRunningAnim(bool bSet)
{
	bPlayingLeftRun = bSet;
	for (int i = 0; i < m_Childs.size(); ++i)
		static_cast<CArmObjectDX11*>(m_Childs[i].get())->SetLeftRunningAnim(bSet);
}

//---------------------------------------------------------------------------------------------------------------------------

CArmObjectDX11::CArmObjectDX11(void* device) : CHierarchyGameObjectDX11(device)
{
}

void CArmObjectDX11::UpdateObject(float elapsedTime)
{
	UpdateAnim(elapsedTime);


}

void CArmObjectDX11::UpdateAnim(float elapsedTime)
{
	if (bPlayingRightRun) {
		UpdateReadyTurnAnim(elapsedTime);
	}
	if (bPlayingLeftRun) {
		UpdateReadyTurnAnim(elapsedTime);
	}
}

void CArmObjectDX11::UpdateReadyTurnAnim(float elapsedTime)
{
	float rotAngle = 70.f * elapsedTime;
	
	if (m_TransformReadyTime < 0) {
		RotateLocalAxis(rotAngle - m_TransfromBodyAngle, 0.f, 0.f);


		m_Rotate += rotAngle;
		if (m_Rotate > 360)
			m_Rotate = 0;


		if (m_Rotate > m_TransformAngle - 1) {
			bPlayingRightRun = false;
			bPlayingLeftRun = false;
			m_TransformReadyTime = m_TransformPrevReadyTime;
		}

		

	}
	--m_TransformReadyTime;
}



void CArmObjectDX11::SetRightRunningAnim(bool bSet)
{
	bPlayingRightRun = bSet;
	
}

void CArmObjectDX11::SetLeftRunningAnim(bool bSet)
{
	bPlayingLeftRun = bSet;
	
}

