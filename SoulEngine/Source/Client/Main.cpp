/*
Entry point for the Client side of this app.
@file Main.cpp
@author Jacob Peterson
@version 1 12/17/20
*/

#include <UtilsLib/CommonTypes.h>
#include <UtilsLib/Macros.h>
#include <UtilsLib/Memory/MemoryManager.h>
#include <UtilsLib/Logger.h>

int main()
{
	Soul::MemoryManager::StartUp(Gigabytes(1));

	SoulLogInfo("Memory used: %d", Soul::MemoryManager::GetTotalAllocatedBytes());
	Soul::MemoryManager::Shutdown();

	return 0;
}