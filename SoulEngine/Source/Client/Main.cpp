/*
Entry point for the Client side of this app.
@file Main.cpp
@author Jacob Peterson
*/

#include <Events/EventBus.h>
#include <UtilsLib/CommonTypes.h>
#include <UtilsLib/Macros.h>
#include <UtilsLib/Memory/MemoryManager.h>
#include <UtilsLib/Memory/UniqueHandle.h>
#include <UtilsLib/Logger.h>
#include <UtilsLib/Timer.h>

#include <TestsLib/TestRunner.h>

void StartUp();
void ShutDown();

//TODO: Crash when cleaning up multiple eventcallbacks that are tied to the same event call

int main()
{
	Soul::Timer timer;
	timer.Start();

	StartUp();

	Soul::TestRunner::RunAllTestSuites();

	// Measure time taken to startup and test
	timer.Stop();
	SoulLogInfo("Time taken:\n\tSeconds: %llf\n\tMillis: %llf\n\tMicros: %llf",
		timer.GetElapsedSeconds(), timer.GetElapsedMilliseconds(), timer.GetElapsedMicroseconds());

	// Clean up
	ShutDown();

	return 0;
}

void StartUp()
{
	Soul::MemoryManager::StartUp(Gigabytes(1), Megabytes(8));
	Soul::EventBus::StartUp(32);
}

void ShutDown()
{
	Soul::EventBus::Shutdown();

	Assert(Soul::MemoryManager::GetTotalAllocatedBytes() == 0);
	Soul::MemoryManager::Shutdown();
}