/*
Timer for profiling code, keeping frame times, etc.
@file Timer.h
@author Jacob Peterson
@edited 12/22/2020
*/

#include "Timer.h"

#include <Windows.h>

namespace Soul
{
	Timer::Timer() :
		_uiElapsedPerformanceCounts(0),
		_uiLastCounterCheckpoint(0),
		_bIsRunning(false)
	{

	}

	void Timer::Start()
	{
		if (!_bIsRunning)
		{
			_bIsRunning = true;

			LARGE_INTEGER oPerformanceCount;
			QueryPerformanceCounter(&oPerformanceCount);
			_uiLastCounterCheckpoint = oPerformanceCount.QuadPart;
		}
	}

	void Timer::Stop()
	{
		AddElapsedPerformanceCounts();
		_bIsRunning = false;
	}

	void Timer::Reset()
	{
		LARGE_INTEGER oPerformanceCount;
		QueryPerformanceCounter(&oPerformanceCount);
		_uiLastCounterCheckpoint = oPerformanceCount.QuadPart;
		_uiElapsedPerformanceCounts = 0;
	}

	Float32 Timer::GetDeltaTime()
	{
		Float32 fMilliseconds = (Float32)GetElapsedTime(0.001);
		Reset();
		return fMilliseconds;
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
		if (_bIsRunning)
		{
			LARGE_INTEGER oPerformanceCount;
			QueryPerformanceCounter(&oPerformanceCount);
			_uiElapsedPerformanceCounts +=
				oPerformanceCount.QuadPart - _uiLastCounterCheckpoint;
			_uiLastCounterCheckpoint = oPerformanceCount.QuadPart;
		}
	}

	Float64 Timer::GetPerformanceFrequency(Float64 fSecondParts)
	{
		LARGE_INTEGER oPerformanceFrequency;
		QueryPerformanceFrequency(&oPerformanceFrequency);
		Float64 fResult = oPerformanceFrequency.QuadPart * fSecondParts;
		return fResult;
	}

	Float64 Timer::GetElapsedTime(Float64 fSecondParts)
	{
		AddElapsedPerformanceCounts();
		Float64 fMicrosecondPerformance = GetPerformanceFrequency(fSecondParts);
		return _uiElapsedPerformanceCounts / fMicrosecondPerformance;
	}
}