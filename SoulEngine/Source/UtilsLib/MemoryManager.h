////////////////////////////////////////////////////////////////////////////////
// File:            MemoryManager.h
//
// Author:          Peterson, Jacob
//
// Last Modified:   Date:   Dec 16 2020
//                  By:     Peterson, Jacob
//                  Exp:    Initial implementation
//
// Purpose:         Reserves an initial amount of memory for the engine to be
//                  used by allocators.
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CommonTypes.h"

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
	};

	////////////////////////////////////////////////////////////////////////////
	// A singleton MemoryManager for the Soul engine. This works by creating a 
	// memory arena that can have memory allocated in it through separate
	// allocator objects. This first needs to be initialized by calling
	// StartUp() (usually done by the engine) and cleaned up via Shutdown().
	//
	// To partition memory, use the Allocate(...) function and provide the
	// needed number of bytes. With the returned pointer, you can deallocate the
	// same memory block by called Deallocate(...).
	//
	// For debugging purposes, the GetTotalAllocatedBytes() and
	// GetTotalFreeBytes() functions can be used to query the current usage
	// of the memory arena's memory.
	////////////////////////////////////////////////////////////////////////////
	class MemoryManager
	{
	public:
		////////////////////////////////////////////////////////////////////////
		// Function:        GetTotalPartitionedBytes
		//
		// Last Modified:   Dec 16 2020
		//
		// Returns:         UInt64 containing the current number of allocated
		//                  bytes.
		////////////////////////////////////////////////////////////////////////
		static UInt64 GetTotalAllocatedBytes();

		////////////////////////////////////////////////////////////////////////
		// Function:        GetTotalFreeBytes
		//
		// Last Modified:   Dec 16 2020
		//
		// Returns:         UInt64 containing the current number of free bytes.
		////////////////////////////////////////////////////////////////////////
		static UInt64 GetTotalFreeBytes();

		////////////////////////////////////////////////////////////////////////
		// Function:        StartUp
		//
		// Last Modified:   Dec 16 2020
		//
		// Input:           uiByteSize   Total amount of bytes to partition for
		//                               this MemoryManager.
		//
		// Notes:           This should be initialized first by the engine.
		////////////////////////////////////////////////////////////////////////
		static void StartUp(ByteCount uiByteSize);

		////////////////////////////////////////////////////////////////////////
		// Function:        Shutdown
		//
		// Last Modified:   Dec 16 2020
		//
		// Notes:           This should be called last by the engine after all
		//                  other systems are shut down.
		////////////////////////////////////////////////////////////////////////
		static void Shutdown();

		////////////////////////////////////////////////////////////////////////
		// Function:        Allocate
		//
		// Last Modified:   Dec 16 2020
		//
		// Input:           iSize   Size of allocation.
		//
		// Returns:         Pointer to allocated memory.
		//
		// Notes:           Used to allocate a specific chunk of memory from the
		//                  memory arena. Typically used by memory allocators.
		////////////////////////////////////////////////////////////////////////
		static void* Allocate(ByteCount uiBytes);

		////////////////////////////////////////////////////////////////////////
		// Function:        Deallocate
		//
		// Last Modified:   Dec 16 2020
		//
		// Input:           pBlockLocation   The location of the block of memory
		//                                   to deallocate.
		//
		// Notes:           Deallocates the allocated block of memory at the
		//                  provided location.
		////////////////////////////////////////////////////////////////////////
		static void Deallocate(AllocationHeader* opBlockLocation);

		////////////////////////////////////////////////////////////////////////
		// Function:        Defragment
		//
		// Last Modified:   Dec 16 2020
		//
		// Input:           uiNodeCount   The number of memory nodes to attempt
		//                                to defragment.
		//
		// Notes:           Shifts the first N nodes up to the highest memory
		//                  address possible to avoid fragmentation.
		////////////////////////////////////////////////////////////////////////
		static void Defragment(UInt8 uiNodeCount);

		// Deleted Functions ///////////////////////////////////////////////////
		
		MemoryManager() = delete;

	private:
		static Byte* _pMemoryStart; // The location of the start of the memory arena.
		static Byte* _pMemoryEnd; // The location of the end of the memory arena.
		static ByteCount _uiByteSize; // The size of this memory arena in bytes.
	};
}