#pragma once
#include "Object.h"

class CCircleObjectDX11 : public CHierarchyGameObjectDX11 {
public:
	CCircleObjectDX11(void* command);
	void UpdateObject(float elapsedTime);
	void Roll();
};

class COrbitObjectDX11 : public CHierarchyGameObjectDX11 {
public:
	COrbitObjectDX11(void* device);
	void UpdateObject(float elapsedTime);
};