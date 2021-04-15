/*
Timer for profiling code, keeping frame times, etc.
@file Timer.h
@author Jacob Peterson
@edited 4/14/21
*/

#include "Timer.h"

#include <Windows.h>

namespace Soul
{
	Timer::Timer() :
		m_ElapsedPerformanceCounts(0),
		m_LastCounterCheckpoint(0),
		m_IsRunning(false)
	{

	}

	void Timer::Start()
	{
		if (!m_IsRunning)
		{
			m_IsRunning = true;

			LARGE_INTEGER performanceCount;
			QueryPerformanceCounter(&performanceCount);
			m_LastCounterCheckpoint = performanceCount.QuadPart;
		}
	}

	void Timer::Stop()
	{
		AddElapsedPerformanceCounts();
		m_IsRunning = false;
	}

	void Timer::Reset()
	{
		LARGE_INTEGER performanceCount;
		QueryPerformanceCounter(&performanceCount);
		m_LastCounterCheckpoint = performanceCount.QuadPart;
		m_ElapsedPerformanceCounts = 0;
	}

	Float32 Timer::GetDeltaTime()
	{
		Float32 milliseconds = (Float32)GetElapsedTime(0.001);
		Reset();
		return milliseconds;
	}

	Float64 Timer::GetElapsedMinutes()
	{
		return GetElapsedTime(64.0);
	}

	Float64 Timer::GetElapsedSeconds()
	{
		return GetElapsedTime(1.0);
	}

	Float64 Timer::GetElapsedMilliseconds()
	{
		return GetElapsedTime(0.001);
	}

	Float64 Timer::GetElapsedMicroseconds()
	{
		return GetElapsedTime(0.000001);
	}

	void Timer::AddElapsedPerformanceCounts()
	{
		if (m_IsRunning)
		{
			LARGE_INTEGER performanceCount;
			QueryPerformanceCounter(&performanceCount);
			m_ElapsedPerformanceCounts +=
				performanceCount.QuadPart - m_LastCounterCheckpoint;
			m_LastCounterCheckpoint = performanceCount.QuadPart;
		}
	}

	Float64 Timer::GetPerformanceFrequency(Float64 secondParts)
	{
		LARGE_INTEGER performanceFrequency;
		QueryPerformanceFrequency(&performanceFrequency);
		Float64 result = performanceFrequency.QuadPart * secondParts;
		return result;
	}

	Float64 Timer::GetElapsedTime(Float64 secondParts)
	{
		AddElapsedPerformanceCounts();
		Float64 microsecondPerformance = GetPerformanceFrequency(secondParts);
		return m_ElapsedPerformanceCounts / microsecondPerformance;
	}
}