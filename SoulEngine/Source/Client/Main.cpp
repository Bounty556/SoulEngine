/*
Entry point for the Client side of this app.
@file Main.cpp
@author Jacob Peterson
*/

#include <UtilsLib/CommonTypes.h>
#include <UtilsLib/Macros.h>
#include <UtilsLib/Memory/MemoryManager.h>
#include <UtilsLib/Memory/UniqueHandle.h>
#include <UtilsLib/Logger.h>

#include <TestsLib/TestRunner.h>

int main()
{
	Soul::MemoryManager::StartUp(Gigabytes(1));

	Soul::TestRunner::RunAllTestSuites();

	Assert(Soul::MemoryManager::GetTotalAllocatedBytes() == 0);
	Soul::MemoryManager::Shutdown();

	return 0;
}