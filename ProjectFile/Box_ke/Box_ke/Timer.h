// ================================================
// Timer.h
// 프레임 측정을 및 고정을 위한 타이머
// ================================================

#pragma once
#include "stdafx.h"

constexpr size_t MAX_FRAME_COUNT = 60;

class CTimer {
public:
	CTimer();

	float Tick(float fps = 0.f);
	int getFPS();
private:
	LARGE_INTEGER		m_Frequency{};
	LARGE_INTEGER		m_StartTime{};
	LARGE_INTEGER		m_EndTime{};

	std::list<float>	m_ElapsedTimesList{};
};