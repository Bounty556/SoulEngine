/*
Reserves an initial amount of memory for the engine to be used by allocators.
@file MemoryManager.h
@author Jacob Peterson
@version 1 12/16/20
*/

#pragma once

#include "CommonTypes.h"

typedef PtrSize ByteCount;

namespace Soul
{
	class MemoryAllocator;
	
	// Placed at the start of each block of free memory in the memory arena.
	struct MemoryNode
	{
		ByteCount uiBlockSize; // Size of the free memory block, including this node.
		MemoryNode* opNextNode; // Location of the following node in memory.
	};

	// Placed at the start of each allocation to track how many bytes are stored.
	struct AllocationHeader
	{
		ByteCount uiBytes; // Bytes stored in this allocation, including this header.
	};

	/*
	A singleton MemoryManager for the Soul engine. This works by creating a 
	memory arena that can have memory allocated in it through separate
	allocator objects. This first needs to be initialized by calling
	StartUp() (usually done by the engine) and cleaned up via Shutdown().
	
	To partition memory for use by an allocator, use the Allocate(...) function
	and provide the needed number of bytes. With the returned pointer, you can
	deallocate the same memory block via Deallocate(...).
	
	For debugging purposes, the GetTotalAllocatedBytes() and
	GetTotalFreeBytes() functions can be used to query the current usage
	of the memory arena's memory.
	*/
	class MemoryManager
	{
	public:

		// Getters /////////////////////////////////////////////////////////////

		/*
		@return UInt64 containing the current number of allocated bytes in this
		        memory arena.
		*/
		static UInt64 GetTotalAllocatedBytes();

		/*
		@return UInt64 containing the current number of free bytes in this
		        memory arena.
		*/
		static UInt64 GetTotalFreeBytes();

		// Member Functions ////////////////////////////////////////////////////

		/*
		Initializes the MemoryManager. This should be called before any other
		MemoryManager function.
		
		@param uiByteSize - The total amount of bytes to partition for this
		                    MemoryManager.
		*/
		static void StartUp(ByteCount uiByteSize);

		/*
		Cleans up the MemoryManager. This should be called last, just before the
		program shuts down.
		*/
		static void Shutdown();

		/*
		Used to allocate a specific chunk of memory from the memory arena.
		Only to be used by MemoryAllocators.

		@param uiBytes - Size of the allocation in bytes.

		@return MemoryAllocator* pointing to the start of the newly allocated
		        block of memory to be used by a MemoryAllocator.
		*/
		static MemoryAllocator* Allocate(ByteCount uiBytes);

		/*
		Deallocates the allocated block of memory at the provided location.

		@param opBlockLocation - The location of the block of memory to
		                         deallocate.
		*/
		static void Deallocate(MemoryAllocator* opBlockLocation);

		/*
		Shifts the first N nodes up to the highest memory address possible to
		avoid fragmentation.

		@param uiNodeCount - The number of memory nodes to attempt to defrag.
		*/
		static void Defragment(UInt8 uiNodeCount);

		// Deleted Functions //////////////////////////////////////////////////
	
		MemoryManager() = delete;

	private:
		static Byte* _pMemoryStart; // The location of the start of the memory arena.
		static Byte* _pMemoryEnd; // The location of the end of the memory arena.
		static ByteCount _uiByteSize; // The size of this memory arena in bytes.
	};
}