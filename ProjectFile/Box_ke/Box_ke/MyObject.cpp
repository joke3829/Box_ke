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
	
}


