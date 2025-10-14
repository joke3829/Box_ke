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