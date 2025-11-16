#include "Animation.h"


CAnimation::CAnimation()
{
}

void CAnimation::AddKey(float time, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT4& rot, const DirectX::XMFLOAT3& scale)
{
	DirectX::XMFLOAT4 normalizedRot;

	DirectX::XMVECTOR q = DirectX::XMQuaternionNormalize(DirectX::XMLoadFloat4(&rot));
	DirectX::XMStoreFloat4(&normalizedRot, q);

	auto k = std::make_shared<KeyframeDesc>();
	k->Time = time;
	k->Position = pos;
	k->RotationQuat = normalizedRot;
	k->Scale = scale;
	KeyAll.push_back(k);


}

void CAnimation::AddKey(float time, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& axis, float angleDegrees, const DirectX::XMFLOAT3& scale)
{
	XMVECTOR q = XMQuaternionRotationAxis(XMLoadFloat3(&axis), XMConvertToRadians(angleDegrees));
	XMFLOAT4 rot;
	XMStoreFloat4(&rot, XMQuaternionNormalize(q));

	auto k = std::make_shared<KeyframeDesc>();
	k->Time = time;
	k->Position = pos;
	k->RotationQuat = rot;
	k->Scale = scale;

	KeyAll.push_back(k);
}




//-----------------------------------------

CAnimPlayer::CAnimPlayer()
{
}

void CAnimPlayer::AddAnimation()
{
}

void CAnimPlayer::AddAnimation(const char* AnimName, shared_ptr<CAnimation> Anim)
{
	AnimationAll.try_emplace(AnimName, Anim);

}


bool  CAnimPlayer::Play(const string& name, bool loop = true)
{
	auto it = AnimationAll.find(name);
	if (it == AnimationAll.end()) 
		return false;
	if (m_currName != name) {
	
		m_lastAnim = m_currAnim;
		m_currAnim = it->second;
		m_currName = name;
	
		m_state.currIndex = 0;
	}
	// 상태 리셋
	m_state.playTime = 0.f;
	m_state.looping = loop;
	m_playing = true;
	return true;
}

void CAnimPlayer::Stop()
{
	m_playing = false;
}

void CAnimPlayer::Update(float elapsedtime)
{
	if (!m_playing || m_currAnim == nullptr) return;
	const auto& keys = m_currAnim->GetKeys();
	size_t keyCount = keys.size();
	if (keyCount == 0) return;


	m_state.playTime += elapsedtime * m_state.speed;

	float lastTime = keys.back()->Time;
	if (m_state.playTime > lastTime) {
		if (m_state.looping && lastTime > 0.0f) {
			m_state.playTime = std::fmod(m_state.playTime, lastTime);
			if (m_state.playTime < 0.f) m_state.playTime += lastTime;
			m_state.currIndex = 0;
		}
		else {
			m_state.playTime = lastTime;
			m_state.currIndex = keyCount - 1;
			m_playing = false;
		}
	}


	while (m_state.currIndex + 1 < keyCount && m_state.playTime >= keys[m_state.currIndex + 1]->Time) {
		++m_state.currIndex;
	}


	auto currKey = keys[m_state.currIndex];
	std::shared_ptr<KeyframeDesc> nextKey = nullptr;
	if (m_state.currIndex + 1 < keyCount) nextKey = keys[m_state.currIndex + 1];

	float ratio = 0.f;
	if (nextKey) {
		float t0 = currKey->Time;
		float t1 = nextKey->Time;
		if (t1 > t0) ratio = (m_state.playTime - t0) / (t1 - t0);
		else ratio = 0.f;
	}
	else {
		ratio = 0.f;
	}

	
	ComputeCurrentPose(currKey, nextKey, ratio);
}

void CAnimPlayer::ComputeCurrentPose(std::shared_ptr<KeyframeDesc> a, shared_ptr<KeyframeDesc> b, float ratio)
{
	if (!a) return;

	if (!b) {
		m_currPose = *a;
		return;
	}

	XMVECTOR pa = XMLoadFloat3(&a->Position);
	XMVECTOR pb = XMLoadFloat3(&b->Position);
	XMVECTOR p = XMVectorLerp(pa, pb, ratio);
	XMStoreFloat3(&m_currPose.Position, p);

	XMVECTOR sa = XMLoadFloat3(&a->Scale);
	XMVECTOR sb = XMLoadFloat3(&b->Scale);
	XMVECTOR s = XMVectorLerp(sa, sb, ratio);
	XMStoreFloat3(&m_currPose.Scale, s);

	XMVECTOR qa = XMLoadFloat4(&a->RotationQuat);
	XMVECTOR qb = XMLoadFloat4(&b->RotationQuat);
	XMVECTOR qr = XMQuaternionSlerp(qa, qb, ratio);
	XMStoreFloat4(&m_currPose.RotationQuat, qr);

	// time
	m_currPose.Time = a->Time + (b->Time - a->Time) * ratio;
}


