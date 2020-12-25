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

int main()
{
	Soul::Timer oTimer;
	oTimer.Start();

	Soul::MemoryManager::StartUp(Gigabytes(1));
	Soul::EventBus::StartUp(32);

	Soul::TestRunner::RunAllTestSuites();

	Soul::EventBus::Shutdown();

	Assert(Soul::MemoryManager::GetTotalAllocatedBytes() == 0);
	Soul::MemoryManager::Shutdown();

	oTimer.Stop();
	SoulLogInfo("Time taken:\n\tSeconds: %llf\n\tMillis: %llf\n\tMicros: %llf",
		oTimer.GetElapsedSeconds(), oTimer.GetElapsedMilliseconds(), oTimer.GetElapsedMicroseconds());

	return 0;
}