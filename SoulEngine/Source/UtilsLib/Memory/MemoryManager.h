/*
Reserves an initial amount of memory for the engine to be used by allocators.
@file MemoryManager.h
@author Jacob Peterson
@version 2 12/17/20
*/

#pragma once

#include <new>

#include <UtilsLib/CommonTypes.h>
#include <UtilsLib/Macros.h>

#define Partition(type, ...) (new (Soul::MemoryManager::Allocate(sizeof(type))) type(__VA_ARGS__))
#define PartitionArray(type, count) ((type*)(Soul::MemoryManager::Allocate(sizeof(type), count)))

typedef PtrSize ByteCount;

namespace Soul
{
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
		UInt16 uiCount; // Number of objects stored in this allocation.
	};

	/*
	A singleton MemoryManager for the Soul engine. This works by creating a 
	memory arena that can have memory allocated in it through separate
	allocator objects. This first needs to be initialized by calling
	StartUp() (usually done by the engine) and cleaned up via Shutdown().
	
	To partition memory for use by an allocator, use the Partition(...) function
	or PartitionArray(...) macros. With the returned pointer, you can deallocate
	the same memory block via Deallocate(...).
	
	For debugging purposes, the GetTotalAllocatedBytes() and GetTotalFreeBytes()
	functions can be used to query the current usage of the memory arena.
	*/
	class MemoryManager
	{
	public:

		// Getters /////////////////////////////////////////////////////////////

		/*
		@return UInt64 containing the current number of allocated bytes in this
		        memory arena.
		*/
		static ByteCount GetTotalAllocatedBytes();

		/*
		@return UInt64 containing the current number of free bytes in this
		        memory arena.
		*/
		static ByteCount GetTotalFreeBytes();

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

		@param uiCount - The number of elements stored in this allocation.

		@return Pointer pointing to the start of the newly allocated block of
		memory to be used by a MemoryAllocator.
		*/
		static void* Allocate(ByteCount uiBytes, UInt16 uiCount = 1);

		/*
		Deallocates the allocated block of memory at the provided location.

		@param pLocation - The location of the block of memory to deallocate.
		*/
		template <class T>
		static void Deallocate(T* pLocation);

		/*
		Prints a small summary of the current memory arena usage to the console.
		*/
		static void PrintMemory();

		// Deleted Functions //////////////////////////////////////////////////
	
		MemoryManager() = delete;

	private:
		
		/*
		Returns a count of how many MemoryNodes exist in the memory arena.
		*/
		static UInt16 CountNodes();

		/*
		Removes the memory node at the provided location, and connects the
		previous and next memory nodes together to repair the list.

		@param opLocation - Pointer to the MemoryNode to be removed.
		*/
		static void RemoveMemoryNode(MemoryNode* opLocation);

		/*
		Attempts to add a new MemoryNode at the location provided. If it
		connects with a previous MemoryNode, it will be absorbed. Otherwise, the
		MemoryNode list will be corrected appropriately.

		@param pLocation - Pointer to the location to create the new MemoryNode
		                    at.
		*/
		static void TryAddingMemoryNode(void* pLocation);

	private:
		static Byte* _suipMemoryStart; // The location of the start of the memory arena.
		static Byte* _suipMemoryEnd; // The location of the end of the memory arena.
		static ByteCount _suiByteSize; // The size of this memory arena in bytes.
		static bool _sbIsSetup; // Whether the MemoryManager has been initialized yet.
	};

	/*
	Calls the destructor on this memory block and marks it as free.
	*/
	template <class T>
	void MemoryManager::Deallocate(T* pLocation)
	{
		Assert(_sbIsSetup);
		Assert(pLocation);

		// Check to see if this is an array we're freeing
		AllocationHeader* opHeader = (AllocationHeader*)((Byte*)pLocation -
			                         sizeof(AllocationHeader));

		int timesToLoop = opHeader->uiCount;
		for (int i = 0; i < timesToLoop; ++i)
		{
			pLocation[i].~T();
		}

		TryAddingMemoryNode(pLocation);
	}
}