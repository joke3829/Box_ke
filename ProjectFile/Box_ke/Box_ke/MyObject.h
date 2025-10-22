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
	void UpdateAnim(float elapsedTime);


public:
	void SetRightRunningAnim(bool bSet);
	void SetLeftRunningAnim(bool bSet);


private:
	bool bPlayingRightRun = false;
	bool bPlayingLeftRun = false;


};
class CBodyObjectDX11 : public CHierarchyGameObjectDX11 {
public:
	CBodyObjectDX11(void* device);
	void UpdateObject(float elapsedTime);

public:
	void UpdateAnim(float elapsedTime);

	void UpdateReadyTurnAnim(float elapsedTime);
	


public:
	void SetRightRunningAnim(bool bSet);
	void SetLeftRunningAnim(bool bSet);;


	void SetTransformAngleAnim(float playspeed) { m_TransformAngle = playspeed;  }
	void SetTransformReadyTime(float playspeed) { 
		m_TransformReadyTime = playspeed; 
		m_TransformPrevReadyTime = playspeed;
	}

private:
	bool bPlayingRightRun = false;
	bool bPlayingLeftRun = false;

	float m_TransformAngle{};
	float m_TransformReadyTime{};
	float m_TransformPrevReadyTime{};

	float m_Rotate{};

};


class CArmObjectDX11 : public CHierarchyGameObjectDX11 {
public:
	CArmObjectDX11(void* device);
	void UpdateObject(float elapsedTime);
	
public:

	void UpdateAnim(float elapsedTime);

	void UpdateReadyTurnAnim(float elapsedTime);

public:
	void SetRightRunningAnim(bool bSet);
	void SetLeftRunningAnim(bool bSet);

	void SetTransformAngleAnim(float playangle) { 
		m_TransformAngle = playangle;
	}
	void SetTransformBodyAngleAnim(float playangle) {
		m_TransfromBodyAngle = playangle;
	}

	void SetTransformReadyTime(float playspeed) { 
		m_TransformReadyTime = playspeed; 
		m_TransformPrevReadyTime = playspeed;
	}

private:
	bool bPlayingRightRun = false;
	bool bPlayingLeftRun = false;

	float m_TransformAngle{};
	float m_TransfromBodyAngle{};
	float m_TransformReadyTime{};
	float m_TransformPrevReadyTime{};

	float m_Rotate{};
};


