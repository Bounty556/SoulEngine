/*
Reserves an initial amount of memory for the engine to be used by allocators.
@file MemoryManager.cpp
@author Jacob Peterson
@version 1 12/17/20
*/

#include "MemoryManager.h"

#include <memory>

namespace Soul
{
	Byte* MemoryManager::_suipMemoryStart;
	Byte* MemoryManager::_suipMemoryEnd;
	ByteCount MemoryManager::_suiByteSize;

	// Getters /////////////////////////////////////////////////////////////////

	/*
	Takes the total memory size and subtracts the total free bytes from it.
	*/
	ByteCount MemoryManager::GetTotalAllocatedBytes()
	{
		return _suiByteSize - GetTotalFreeBytes();
	}

	/*
	Hops from MemoryNode to MemoryNode summing up the total free bytes.
	*/
	ByteCount MemoryManager::GetTotalFreeBytes()
	{
		MemoryNode* opCurrentNode = (MemoryNode*)_suipMemoryStart;
		ByteCount uiTotalFreeBytes = 0;

		/*
		Traverse MemoryNodes.
		*/
		while (true)
		{
			uiTotalFreeBytes += opCurrentNode->uiBlockSize;
			if (opCurrentNode->opNextNode)
			{
				opCurrentNode = opCurrentNode->opNextNode;
			}
			else
			{
				break;
			}
		}
		
		return uiTotalFreeBytes;
	}

	/*
	Reserves the provided amount of memory and initializes the MemoryNodes.
	*/
	void MemoryManager::StartUp(ByteCount uiByteSize)
	{
		_suipMemoryStart = (Byte*)malloc(uiByteSize);
		_suipMemoryEnd = _suipMemoryStart + uiByteSize;

		/*
		Create the initial MemoryNode at the start of the memory arena.
		*/
		MemoryNode* opStartNode = (MemoryNode*)_suipMemoryStart;
		opStartNode->opNextNode = nullptr;
		opStartNode->uiBlockSize = uiByteSize;
	}

	/*
	Frees all of the memory in this MemoryManager.
	*/
	void MemoryManager::Shutdown()
	{
		free(_suipMemoryStart);
	}

	/*
	Attempt to allocate a block of memory with the given size in the memory
	arena.
	*/
	MemoryAllocator* MemoryManager::Allocate(ByteCount uiBytes)
	{
		/*
		Find the earliest block of memory with enough size to accomodate this
		allocation.
		*/
		MemoryNode* opStartNode = (MemoryNode*)_suipMemoryStart;
		while (opStartNode && opStartNode->uiBlockSize < uiBytes)
		{
			opStartNode = opStartNode->opNextNode;
		}

		/*
		We successfully found a node that can accomodate the requested memory.
		*/
		if (opStartNode)
		{

		}
		/*
		No adequate memory block found.
		*/
		else
		{
			return nullptr;
		}
	}
}