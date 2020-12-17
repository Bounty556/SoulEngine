/*
Reserves an initial amount of memory for the engine to be used by allocators.
@file MemoryManager.h
@author Jacob Peterson
@version 3 12/17/20
*/

#pragma once

#include <new>

#include <UtilsLib/CommonTypes.h>
#include <UtilsLib/Macros.h>

typedef unsigned char ByteCount;

namespace Soul
{
	template <class T>
	class UniqueHandle;

	/*
	Returned when allocating memory. Should be used in a UniqueHandle object.
	*/
	struct Handle
	{
		Handle* opNextHandle; // The handle closest to this one.
		Byte* uipLocation; // Location that this handle points to in the memory arena.
		ByteCount uiByteSize; // Size of the memory block that this handle points to.
		UInt32 uiElementCount; // Number of elements allocated in the memory block.
		bool bIsUsed; // Whether this handle is currently in use.
	};

	/*
	A singleton MemoryManager for the Soul engine. This first needs to be
	initialized by calling StartUp() (usually done by the engine) and cleaned up
	via Shutdown().

	To partition memory for use, call Allocate<T>(...) with the type desired
	inserted into the template. This will return a new UniqueHandle which can
	then be used to access the memory block that was allocated for the object.

	For debugging purposes, the GetTotalAllocatedBytes() and GetTotalFreeBytes()
	functions can be used to query the current usage of the memory arena. The
	PrintMemory() function also prints out a brief summary of the current memory
	usage.
	*/
	class MemoryManager
	{
	public:

		/*
		Initializes the MemoryManager's memory and sets up the Handle table.
		*/
		static void StartUp();

		/*
		Shuts down the MemoryManager and frees all its memory.
		*/
		static void Shutdown();

		/*
		Attempts to allocate the provided amount of memory in the arena. The
		total amount of memory reserved will be equal to uiBytes * uiCount.

		@param uiBytes - The number of bytes needed to reserve one element of
						 the provided type.

		@param uiCount - The number of elements to reserve memory for.

		@return UniqueHandle<T> containing the handle that points to the newly
		                        allocated memory.
		*/
		template <class T>
		static UniqueHandle<T> Allocate(ByteCount uiBytes, UInt32 uiCount);

		/*
		Calls the destructor and frees the memory for every object allocated to
		the provided handle.

		@param oHandle - The handle whose memory needs to be freed.
		*/
		template <class T>
		static void Deallocate(Handle& oHandle);

		/*
		Attempts to defragment the provided number of blocks to keep memory
		contiguous and cache-friendly.

		@param uiBlockCount - The number of memory blocks to attempt to defrag.
		*/
		static void Defragment(UInt8 uiBlockCount);

		/*
		Returns the total number of bytes that have been allocated by the
		MemoryManager (this does not include the memory used by the Handle table)
		*/
		static void GetTotalAllocatedBytes();

		/*
		Returns the total number of bytes that are free in the MemoryManager.
		*/
		static void GetTotalFreeBytes();

	private:
		MemoryManager() = delete;

	private:
		static Byte* _suipMemoryStart; // Start of partitioned memory.
		static Byte* _suipAddressableMemoryStart; // Start of memory that can be allocated to.
		static Byte* _suipMemoryEnd; // End of addressable memory.
		static UInt32 _uiHandleTableLength; // Maximum amount of handles that can be created.
		static Handle* _opHandleTableStart; // Start address of handle table.
		static bool _bIsSetup; // Whether this MemoryManager has been initialized yet.
	};
}