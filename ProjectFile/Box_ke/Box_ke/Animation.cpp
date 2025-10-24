#include "Animation.h"

bool Channel::ChangeInterAnim(float t, XMVECTOR& outPos, XMVECTOR& outQuat, XMVECTOR& outScale) const
{
	if (Keys.empty()) return false;

	if (t <= Keys.front().Time) {
		outPos = XMLoadFloat3(&Keys.front().Position);
		outQuat = XMLoadFloat4(&Keys.front().RotationQuat);
		outScale = XMLoadFloat3(&Keys.front().Scale);

		return true;
	}

	if (t >= Keys.back().Time) {
		outPos = XMLoadFloat3(&Keys.back().Position);
		outQuat = XMLoadFloat4(&Keys.back().RotationQuat);
		outScale = XMLoadFloat3(&Keys.back().Scale);
		return true;
	}

	for (size_t i = 0; i + 1 < Keys.size(); ++i) {
		const KeyFrame& a = Keys[i]; // 앞에 키
		const KeyFrame& b = Keys[i+1]; // 뒤에 키

		if (t >= a.Time && t <= b.Time) {
			float localT = (t - a.Time) / (b.Time - a.Time);
			XMVECTOR pa = XMLoadFloat3(&a.Position);
			XMVECTOR pb = XMLoadFloat3(&b.Position);
			outPos = XMVectorLerp(pa, pb, localT);

			XMVECTOR qa = XMLoadFloat4(&a.RotationQuat);
			XMVECTOR qb = XMLoadFloat4(&b.RotationQuat);
			outQuat = XMQuaternionSlerp(qa, qb, localT);

			XMVECTOR sa = XMLoadFloat3(&a.Scale);
			XMVECTOR sb = XMLoadFloat3(&b.Scale);
			outScale = XMVectorLerp(sa, sb, localT);
			return true;
		}
		
	}
	return false;
}

//----------------------------------------------------------------------------------------------------------------

CAnimation::CAnimation()
{
}

void CAnimation::Play(AnimationClip* clip, bool loop)
{
	m_Clip = clip;
	m_Time = 0.0f;
	m_Playing = (clip != nullptr);
	m_Loop = loop;
	m_InTransition = false;
	m_ClipFrom = m_ClipTo = nullptr;
}

void CAnimation::stop()
{
	m_Playing = false;
	m_Clip = nullptr;
	m_InTransition = false;
	m_ClipFrom = m_ClipTo = nullptr;
	m_Time = 0.0f;
}

void CAnimation::Update(float deltaTime)
{
	if (!m_Playing) return;
	if (m_InTransition) {
		// advance both times
		m_TimeFrom += deltaTime * m_Speed;
		m_TimeTo += deltaTime * m_Speed;
		m_TransTime += deltaTime;
		if (m_TransTime >= m_TransDuration) {
			m_Clip = m_ClipTo;
			m_Time = m_TimeTo;
			m_InTransition = false;
			m_ClipFrom = m_ClipTo = nullptr;
		}
	}
	else {
		m_Time += deltaTime * m_Speed;
		if (m_Clip && m_Clip->fDuration > 0.0f) {
			if (m_Loop) {
				while (m_Time > m_Clip->fDuration) m_Time -= m_Clip->fDuration;
			}
			else {
				if (m_Time > m_Clip->fDuration) { m_Time = m_Clip->fDuration; m_Playing = false; }
			}
		}
	}
}

void CAnimation::StartTransition(AnimationClip* toClip, float duration)
{
	if (!m_Playing || !m_Clip) {
	
		Play(toClip, true);
		return;
	}
	m_InTransition = true;
	m_ClipFrom = m_Clip;
	m_ClipTo = toClip;
	m_TimeFrom = m_Time;
	m_TimeTo = 0.0f;
	if (duration > 0.f)
		m_TransDuration = duration;
	else 
		m_TransDuration = 0.001f;

	m_TransTime = 0.0f;
}

