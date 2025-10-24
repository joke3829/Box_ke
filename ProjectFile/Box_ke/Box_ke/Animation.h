#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include <DirectXMath.h>

using namespace DirectX;
using namespace std;

struct KeyFrame {
	float Time = 0.f;
	XMFLOAT3 Position{ 0,0,0 };
	XMFLOAT4 RotationQuat{ 0,0,0 ,1};
	XMFLOAT3 Scale{ 1,1,1 };
};


class Channel {
public:
	vector<KeyFrame> Keys;

	bool ChangeInterAnim(float t, XMVECTOR& outPos, XMVECTOR& outQuat, XMVECTOR& outScale) const;
};

class AnimationClip {
public:
	string name;
	float fDuration{};
	bool bLoop = true;
	unordered_map<string, Channel> Channels;
};

class CAnimation
{
public:
	CAnimation();

public:
	void Play(AnimationClip* clip, bool loop = true);
	void stop();
	bool IsPlaying() const { return m_Playing; }

	void Update(float deltaTime);
	void StartTransition(AnimationClip* toClip, float duration);

private:
	AnimationClip* m_Clip = nullptr;
	float m_Time = 0.f;
	bool m_Playing = false;
	float m_Speed = 1.f;
	bool m_Loop = true;

	bool m_InTransition = false;
	AnimationClip* m_ClipFrom = nullptr;
	AnimationClip* m_ClipTo = nullptr;

	float m_TimeFrom = 0.f;
	float m_TimeTo = 0.f;
	float m_TransDuration = 0.f;
	float m_TransTime = 0.f;
	
	


};

