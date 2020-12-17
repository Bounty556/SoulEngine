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

		// TODO: Change this to find the *last* block with enough size
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
			memset(opHeader, 0, uiActualStoredBytes);
			opHeader->uiBytes = uiActualStoredBytes;
			uipBlockLocation += sizeof(AllocationHeader);

			return uipBlockLocation;
		}
		else if (opCurrentNode->uiBlockSize >= uiActualStoredBytes)
		{
			/*
			Save new block size before erasing block.
			*/
			ByteCount uiBlockSize = opCurrentNode->uiBlockSize;
			RemoveMemoryNode(opCurrentNode);
			memset(opCurrentNode, 0, uiBlockSize);

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
		Assert(_sbIsSetup);

		// TODO:
		//opBlockLocation->~MemoryAllocator();
		TryAddingMemoryNode(opBlockLocation);
	}

	/*
	Shifts the first N nodes up to the highest memory address possible to avoid
	fragmentation.
	*/
	void MemoryManager::Defragment(UInt8 uiNodeCount)
	{
		Assert(_sbIsSetup);

		// TODO: Implement
	}

	/*
	Removes the memory node at the provided location, and connects the previous
	and next memory nodes together to repair the list.
	*/
	void MemoryManager::RemoveMemoryNode(MemoryNode* opLocation)
	{
		Assert(_sbIsSetup);

		/*
		Find the node just before the node we need to remove, so we can sew the
		list back together.
		*/
		MemoryNode* opCurrentNode = (MemoryNode*)_suipMemoryStart;
		while (opCurrentNode->opNextNode &&
			   opCurrentNode->opNextNode != opLocation)
		{
			opCurrentNode = opCurrentNode->opNextNode;
		}
		Assert(opCurrentNode->opNextNode);
		
		opCurrentNode->opNextNode = opCurrentNode->opNextNode->opNextNode;
	}

	/*
	Attempts to add a new MemoryNode at the location provided. If it connects
	with a previous MemoryNode, it will be absorbed. Otherwise, the MemoryNode
	list will be corrected appropriately.
	*/
	void MemoryManager::TryAddingMemoryNode(void* opLocation)
	{
		Assert(_sbIsSetup);

		/*
		Find between which nodes this should new node should be added.
		*/
		MemoryNode* opPrevNode = (MemoryNode*)_suipMemoryStart;
		MemoryNode* opNextNode = opPrevNode->opNextNode;
		while (opNextNode && opNextNode < opLocation)
		{
			opPrevNode = opNextNode;
			opNextNode = opNextNode->opNextNode;
		}

		/*
		Create the node at opLocation, connected to opPrevNode and opNextNode.
		*/
		AllocationHeader* opPrevHeader =
			(AllocationHeader*)((Byte*)opLocation - sizeof(AllocationHeader));
		ByteCount uiBlockSize = opPrevHeader->uiBytes;
		MemoryNode* opNewNode = (MemoryNode*)opPrevHeader;
		opNewNode->uiBlockSize = uiBlockSize;
		opNewNode->opNextNode = opNextNode;
		opPrevNode->opNextNode = opNewNode;

		/*
		Check to see if this new node can be absorbed.
		*/
		if ((Byte*)opPrevNode + opPrevNode->uiBlockSize == (Byte*)opNewNode)
		{
			opPrevNode->uiBlockSize += opNewNode->uiBlockSize;
			opPrevNode->opNextNode = opNewNode->opNextNode;
		}
	}
}