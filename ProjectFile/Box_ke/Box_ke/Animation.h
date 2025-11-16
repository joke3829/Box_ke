#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include <DirectXMath.h>




using namespace DirectX;
using namespace std;

struct KeyframeDesc
{
	float Time = 0.f;

	DirectX::XMFLOAT3 Position{ 0,0,0 };
	DirectX::XMFLOAT4 RotationQuat{ 0,0,0,1 };
	DirectX::XMFLOAT3 Scale{ 1,1,1 };
};

struct PlayState {
	float playTime = 0.f;       
	size_t currIndex = 0;   
	float speed = 1.0f;        
	bool looping = true;        

};

class CAnimation {
public:
	CAnimation();

	
public:
	void AddKey(float time, const DirectX::XMFLOAT3& pos,
		const DirectX::XMFLOAT4& rot, const DirectX::XMFLOAT3& scale);

	void AddKey(float time, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& axis, float angleDegrees, const DirectX::XMFLOAT3& scale);

	

	size_t GetKeyCount() const { 
		return KeyAll.size(); 
	}

	std::shared_ptr<KeyframeDesc> GetKey(size_t i) const
	{
		if (i < KeyAll.size()) 
			return KeyAll[i];
		return nullptr;
	}

	const std::vector<std::shared_ptr<KeyframeDesc>>& GetKeys() const { return KeyAll; }

private:
	vector<shared_ptr<KeyframeDesc>> KeyAll;

};


class CAnimPlayer {
public:
	CAnimPlayer();

public:
	void AddAnimation();
	void AddAnimation(const char* AnimName, shared_ptr<CAnimation> Anim);

	void SetCurrentPose(float x, float y, float z) {
		m_currPose.Position.x += x;
		m_currPose.Position.y += y;
		m_currPose.Position.z += z;
	}

	void SetCurrentRotationQuat(float x, float y, float z) {
		XMVECTOR qCurr = XMLoadFloat4(&m_currPose.RotationQuat);

		XMVECTOR qDelta =
			XMQuaternionRotationRollPitchYaw(
				XMConvertToRadians(x),
				XMConvertToRadians(y),
				XMConvertToRadians(z)
			);

		XMVECTOR qResult = XMQuaternionMultiply(qCurr, qDelta);
		qResult = XMQuaternionNormalize(qResult);
		XMStoreFloat4(&m_currPose.RotationQuat, qResult);
	}


	shared_ptr<CAnimation> GetAnimation(const string& name)
	{
		auto it = AnimationAll.find(name);
		if (it != AnimationAll.end())
			return it->second;
		return nullptr;
	}
	string	GetAnimationName() {
		return m_currName;
	}
	KeyframeDesc GetCurrentPose() const {
		return m_currPose;
	}

	bool GetPlaying() {
		return m_playing;
	}

public:

	bool  Play(const string& name, bool loop);
	void Stop();
	void Update(float elapsedtime);

	void ComputeCurrentPose(shared_ptr<KeyframeDesc> a, shared_ptr<KeyframeDesc> b, float ratio);
private:
	unordered_map<string, shared_ptr<CAnimation>> AnimationAll;
	std::shared_ptr<CAnimation> m_currAnim{};
	std::shared_ptr<CAnimation> m_lastAnim{};
	string m_currName{};

	PlayState m_state{};
	KeyframeDesc m_currPose{};
	bool m_playing = false;
};
