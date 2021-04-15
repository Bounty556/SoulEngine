/*
Timer for profiling code, keeping frame times, etc.
@file Timer.h
@author Jacob Peterson
@edited 4/14/21
*/

#pragma once

#include <UtilsLib/CommonTypes.h>

namespace Soul
{
	/*
	Used to keep track of an elapsed amount of time. The timer can be started
	with Start() is called, and can be stopped with Stop(). At any time, whether
	the timer is running or not, you can retrieve the current time that has
	passed in many time units by using the corresponding get function. For
	example, to get the elapsed number of milliseconds, call
	GetElapsedMilliseconds().
	*/
	class Timer
	{
	public:
		Timer();

		/*
		Starts the timer.
		*/
		void Start();

		/*
		Stops the timer.
		*/
		void Stop();

		/*
		Resets this timer to have no elapsed time.
		*/
		void Reset();

		/*
		Returns the length of time this timer has been running in milliseconds
		and resets the counter to 0.

		@return Float32 containing the number of elapsed milliseconds.
		*/
		Float32 GetDeltaTime();

		/*
		Returns the length of time this time has been running in minutes.

		@return Float64 containing the number of elapsed minutes.
		*/
		Float64 GetElapsedMinutes();

		/*
		Returns the length of time this timer has been running in seconds.

		@return Float64 containing the number of elapsed seconds.
		*/
		Float64 GetElapsedSeconds();

		/*
		Returns the length of time this timer has been running in milliseconds.

		@return Float64 containing the number of elapsed milliseconds.
		*/
		Float64 GetElapsedMilliseconds();

		/*
		Returns the length of time this timer has been running in microseconds.

		@return Float64 containing the number of elapsed microseconds.
		*/
		Float64 GetElapsedMicroseconds();

	private:
		/*
		If the timer is running, add the number of elapsed performance counts
		since the last counter checkpoint to our performance counter.
		*/
		void AddElapsedPerformanceCounts();

		/*
		Queries the performance frequency and returns it in seconds / secondParts.
		For example, to get the performance frequency in milliseconds, pass in
		1/1000 or 0.001.

		@param secondParts - What to multiply the performance frequency by so
		                      we get the correct time unit back.

		@return Float64 containing the performance frequency in the desired
		        time unit.
		*/
		Float64 GetPerformanceFrequency(Float64 secondParts);

		/*
		Returns the elapsed amount of time in the time units based on
		secondParts. For example, to get the time in milliseconds, pass in
		1/1000 or 0.001.

		@param secondParts - The resulting time unit to return the elapsed time
		                      in.

		@return Float64 containing the elapsed time in the desired time unit.
		*/
		Float64 GetElapsedTime(Float64 secondParts);

	private:
		UInt64 m_ElapsedPerformanceCounts; // Actual elapsed performance counts.
		UInt64 m_LastCounterCheckpoint; // Updated everytime we start, stop, or retrieve current time.
		bool m_IsRunning; // Whether this timer is currently running.
	};
}