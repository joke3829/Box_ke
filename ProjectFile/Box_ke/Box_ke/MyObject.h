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


//-----------------------------------------------------------------------------

class CHeroObjectDX11 : public CHierarchyGameObjectDX11 {
public:
	CHeroObjectDX11(void* device);
	void UpdateObject(float elapsedTime);
	

public:
	void Initialize();
	void UpdateAnim(float elapsedTime);

	shared_ptr<CAnimPlayer> GetAnimPlayer(){
		return m_Animplayer;
	}
private:
	shared_ptr<CAnimPlayer> m_Animplayer;

};

class CBodyObjectDX11 : public CHierarchyGameObjectDX11 {
public:
	CBodyObjectDX11(void* device);
	void UpdateObject(float elapsedTime);

public:
	void Initialize();
	void UpdateAnim(float elapsedTime);

private:
	shared_ptr<CAnimPlayer> m_Animplayer;


};


class CArmObjectDX11 : public CHierarchyGameObjectDX11 {
public:
	CArmObjectDX11(void* device);
	void UpdateObject(float elapsedTime);
	
public:
	void Initialize();
	void UpdateAnim(float elapsedTime);

private: 
	shared_ptr<CAnimPlayer> m_Animplayer;

	
};


