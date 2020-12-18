/*
Entry point for the Client side of this app.
@file Main.cpp
@author Jacob Peterson
@version 1 12/17/20
*/

#include <UtilsLib/CommonTypes.h>
#include <UtilsLib/Macros.h>
#include <UtilsLib/Memory/MemoryManager.h>
#include <UtilsLib/Memory/UniqueHandle.h>
#include <UtilsLib/Logger.h>

int main()
{
	Soul::MemoryManager::StartUp(Gigabytes(1));

	{
		Soul::UniqueHandle<int> int1 = Soul::MemoryManager::Allocate<int>();
		Soul::UniqueHandle<int> int2 = Soul::MemoryManager::Allocate<int>();

		int1 = std::move(int2);

		Soul::MemoryManager::Defragment(1);
	}

	Soul::MemoryManager::PrintMemory();
	Assert(Soul::MemoryManager::GetTotalAllocatedBytes() == 0);
	Soul::MemoryManager::Shutdown();

	return 0;
}