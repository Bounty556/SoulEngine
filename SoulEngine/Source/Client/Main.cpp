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

#include <UtilsLib/Containers/Queue.h>

int main()
{
	Soul::MemoryManager::StartUp(Gigabytes(1));

	Soul::Queue<int> intQueue(32);

	for (UInt8 i = 0; i < intQueue.GetCapacity(); ++i)
	{
		intQueue.Push(i);
	}

	for (UInt8 i = 0; i < intQueue.GetCapacity(); ++i)
	{
		SoulLogInfo("%d", intQueue.Pop());
	}

	Assert(Soul::MemoryManager::GetTotalAllocatedBytes() == 0);
	Soul::MemoryManager::Shutdown();

	return 0;
}