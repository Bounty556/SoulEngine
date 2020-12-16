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

// Includes ////////////////////////////////////////////////////////////////////
#include "CommonTypes.h"

// Structs /////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Struct:          MemoryNode
//
// Last Modified:   Dec 16 2020
//
// Purpose:         Placed at the start of each block of free memory in the
//                  memory arena.
////////////////////////////////////////////////////////////////////////////////
struct MemoryNode
{
	UInt64 BlockSize; // Size of the free memory block including this node
	MemoryNode* poNextNode; // Location of the following node in memory
};

////////////////////////////////////////////////////////////////////////////////
// Struct:          MemoryNode
//
// Last Modified:   Dec 16 2020
//
// Purpose:         Placed at the start of each allocation to track how many
//                  bytes are stored.
////////////////////////////////////////////////////////////////////////////////
struct PartitionHeader
{
	UInt64 Bytes; // Bytes stored in this partition, including this header
	UInt16 Count; // Objects store in this partition
};

// Classes /////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Class:           MemoryManager
//
// Last Modified:   Dec 16 2020
//
// Purpose:         Reserves an initial amount of memory for the engine to be
//                  used by allocators.
////////////////////////////////////////////////////////////////////////////////
class MemoryManager
{
public:
	////////////////////////////////////////////////////////////////////////////
	// Function:        GetSingleton
	//
	// Last Modified:   Dec 16 2020
	//
	// Returns:         A reference a singleton instance of the MemoryManager.
	//
	// Notes:           If StartUp() has not been called first before this,
	//                  StartUp() will be called.
	////////////////////////////////////////////////////////////////////////////
	static MemoryManager& GetSingleton();
	
	////////////////////////////////////////////////////////////////////////////
	// Function:        GetTotalPartitionedBytes
	//
	// Last Modified:   Dec 16 2020
	//
	// Returns:         UInt64 containing the current number of allocated bytes.
	////////////////////////////////////////////////////////////////////////////
	UInt64 GetTotalAllocatedBytes();

	////////////////////////////////////////////////////////////////////////////
	// Function:        GetTotalFreeBytes
	//
	// Last Modified:   Dec 16 2020
	//
	// Returns:         UInt64 containing the current number of free bytes.
	////////////////////////////////////////////////////////////////////////////
	UInt64 GetTotalFreeBytes();

	////////////////////////////////////////////////////////////////////////////
	// Function:        StartUp
	//
	// Last Modified:   Dec 16 2020
	//
	// Input:           uiByteSize   Total amount of bytes to partition for this
	//                               MemoryManager.
	//
	// Notes:           This should be initialized first by the engine.
	////////////////////////////////////////////////////////////////////////////
	void StartUp(UInt64 uiByteSize);

	////////////////////////////////////////////////////////////////////////////
	// Function:        Shutdown
	//
	// Last Modified:   Dec 16 2020
	//
	// Notes:           This should be called last by the engine after all other
	//                  systems are shut down.
	////////////////////////////////////////////////////////////////////////////
	void Shutdown();

    ////////////////////////////////////////////////////////////////////////////
    // Function:        GetSingleton
    //
    // Last Modified:   Dec 16 2020
    //
    // Input:           iSize   Size of allocation.
    //
    // Returns:         Pointer to allocated memory.
    //
    // Notes:           Used to allocate a specific chunk of memory from the
	//                  memory arena. Typically used by memory allocators.
    ////////////////////////////////////////////////////////////////////////////
	void* Allocate(UInt64 uiBytes);

	////////////////////////////////////////////////////////////////////////////
	// Function:        Deallocate
	//
	// Last Modified:   Dec 16 2020
	//
	// Input:           pBlockLocation   
	//
	// Returns:         Pointer to allocated memory.
	//
	// Notes:           Used to allocate a specific chunk of memory from the
	//                  memory arena. Typically used by memory allocators.
	////////////////////////////////////////////////////////////////////////////
	void Deallocate(void* pBlockLocation);

	void Defragment(UInt8 uiNodeCount);

private:
	MemoryManager();

	void AddNode(void* pLocation, UInt64 uiSize);
	void RemoveNode(void* pLocation);

private:
	Byte* _pMemoryStart;
	Byte* _pMemoryEnd;
};