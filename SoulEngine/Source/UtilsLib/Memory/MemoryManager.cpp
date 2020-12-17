/*
Reserves an initial amount of memory for the engine to be used by allocators.
@file MemoryManager.cpp
@author Jacob Peterson
@version 1 12/17/20
*/

#include "MemoryManager.h"

#include <memory>

#include <UtilsLib/Logger.h>
#include <UtilsLib/Macros.h>

namespace Soul
{
	Byte* MemoryManager::_suipMemoryStart;
	Byte* MemoryManager::_suipMemoryEnd;
	ByteCount MemoryManager::_suiByteSize;
	bool MemoryManager::_sbIsSetup;

	// Getters /////////////////////////////////////////////////////////////////

	/*
	Takes the total memory size and subtracts the total free bytes from it.
	*/
	ByteCount MemoryManager::GetTotalAllocatedBytes()
	{
		Assert(_sbIsSetup);
		return _suiByteSize - GetTotalFreeBytes();
	}

	/*
	Hops from MemoryNode to MemoryNode summing up the total free bytes.
	*/
	ByteCount MemoryManager::GetTotalFreeBytes()
	{
		Assert(_sbIsSetup);
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
		_sbIsSetup = true;

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
		Assert(_sbIsSetup);
		free(_suipMemoryStart);
	}

	/*
	Attempt to allocate a block of memory with the given size in the memory
	arena.
	*/
	void* MemoryManager::Allocate(ByteCount uiBytes)
	{
		Assert(_sbIsSetup);

		/*
		Find the earliest block of memory with enough size to accomodate this
		allocation.
		*/
		ByteCount uiActualStoredBytes = uiBytes + sizeof(AllocationHeader);
		MemoryNode* opCurrentNode = (MemoryNode*)_suipMemoryStart;
		while (opCurrentNode && opCurrentNode->uiBlockSize < uiActualStoredBytes)
		{
			opCurrentNode = opCurrentNode->opNextNode;
		}
		Assert(opCurrentNode);
	
		/*
		There are two scenarios:
		1. We have enough memory in this block to store both the memory node and
		   the requested memory block.
		2. We have enough memory in this block to store the memory block, but we
		   will have to delete the memory node to make room for it.
		*/
		if (opCurrentNode->uiBlockSize - sizeof(MemoryNode) >= uiActualStoredBytes)
		{
			Byte* uipBlockLocation =
				(Byte*)opCurrentNode + opCurrentNode->uiBlockSize - uiActualStoredBytes;
			opCurrentNode->uiBlockSize -= uiActualStoredBytes;

			/*
			Create AllocationHeader in place.
			*/
			AllocationHeader* opHeader = (AllocationHeader*)uipBlockLocation;
			opHeader->uiBytes = uiActualStoredBytes;
			uipBlockLocation += sizeof(AllocationHeader);

			return uipBlockLocation;
		}
		else if (opCurrentNode->uiBlockSize >= uiActualStoredBytes)
		{
			RemoveMemoryNode(opCurrentNode);
			ByteCount uiBlockSize = opCurrentNode->uiBlockSize;
			AllocationHeader* opHeader = (AllocationHeader*)opCurrentNode;
			opHeader->uiBytes = uiBlockSize;
			Byte* uipBlockLocation = (Byte*)opHeader + sizeof(AllocationHeader);
			return uipBlockLocation;
		}
		else
		{
			SoulLogError("Error allocating %d bytes of memory.", uiBytes);
			Assert(false);
		}
	}

	/*
	Calls the destructor on this memory block and marks it as free.
	*/
	void MemoryManager::Deallocate(MemoryAllocator* opBlockLocation)
	{
		opBlockLocation->~MemoryAllocator();
		TryAddingMemoryNode(opBlockLocation);
	}
}