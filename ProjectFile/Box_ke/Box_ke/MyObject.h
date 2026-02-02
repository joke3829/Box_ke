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

class CBoxObjectDX11 : public CHierarchyGameObjectDX11 {
public:
	CBoxObjectDX11(void* command);
	void UpdateObject(float elapsedTime);
	void Roll();
};

//-----------------------------------------------------------------------------

class CHeroObjectDX11 : public CHierarchyGameObjectDX11 {
public:
	CHeroObjectDX11(void* device);
	void UpdateObject(float elapsedTime);
	

public:
	void Initialize();

	virtual shared_ptr<CAnimPlayer> GetAnimPlayer(){
		return m_Animplayer;
	}
protected:
	shared_ptr<CAnimPlayer> m_Animplayer;

};

class CBodyObjectDX11 : public CHeroObjectDX11 {
public:
	CBodyObjectDX11(void* device);
	void UpdateObject(float elapsedTime);

public:
	void Initialize();

};


class CGunObjectDX11 : public CHeroObjectDX11 {
public:
	CGunObjectDX11(void* device);
	void UpdateObject(float elapsedTime);




public:
	void SetHero(shared_ptr<CGameObject> hero) { m_Hero = hero; }
	
	shared_ptr<CAnimPlayer> GetGunAnimPlayer() const { return m_GunAnimplayer; }
public:
	void Initialize();

protected:

	shared_ptr<CGameObject> m_Hero;
	shared_ptr<CAnimPlayer> m_GunAnimplayer;

	enum GUN_STATE { GUN_IDLE, GUN_ANIMATING };
	GUN_STATE m_State = GUN_IDLE;



	
};


