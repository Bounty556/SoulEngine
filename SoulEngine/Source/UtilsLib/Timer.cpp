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
		AddElapsedPerformanceCounts();
		Float64 fMillisecondPerformance = GetPerformanceFrequency(0.001);
		Float32 elapsedTime =
			_uiElapsedPerformanceCounts / fMillisecondPerformance;
		Reset();
		return elapsedTime;
	}

	Float64 Timer::GetElapsedMilliseconds()
	{
		AddElapsedPerformanceCounts();
		Float64 fMillisecondPerformance = GetPerformanceFrequency(0.001);
		return _uiElapsedPerformanceCounts / fMillisecondPerformance;
	}

	Float64 Timer::GetElapsedMicroseconds()
	{
		AddElapsedPerformanceCounts();
		Float64 fMicrosecondPerformance = GetPerformanceFrequency(0.000001);
		return _uiElapsedPerformanceCounts / fMicrosecondPerformance;
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
}