/*
Reserves an initial amount of memory for the engine to be allocated as needed.
@file MemoryManager.h
@author Jacob Peterson
@version 3 12/18/20
*/

#pragma once

#include <utility>
#include <new>

#include <UtilsLib/CommonTypes.h>
#include <UtilsLib/Macros.h>

typedef PtrSize ByteCount;

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
		void* pLocation; // Location that this handle points to in the memory arena.
		ByteCount uiByteSize; // Size of the memory block that this handle points to.
		UInt32 uiElementCount; // Number of elements allocated in the memory block.
		bool bIsUsed; // Whether this handle is currently in use.
	};

	/*
	A singleton MemoryManager for the Soul engine. This first needs to be
	initialized by calling StartUp() (usually done by the engine) and cleaned up
	via Shutdown().

	To partition memory for use, call Allocate<T>(...) or AllocateArray<T> with
	the desired type inserted into the template. This will return a new
	UniqueHandle which can then be used to access the memory block that was
	allocated for the object.

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

		@param uiByteSize - The number of bytes to reserve for this MemoryManager.
		*/
		static void StartUp(ByteCount uiByteSize);

		/*
		Shuts down the MemoryManager and frees all its memory.
		*/
		static void Shutdown();

		/*
		Attempts to allocate memory for the provided object type.

		@param oArgs - The arguments to initialize the object with.

		@return UniqueHandle<T> containing the handle that points to the newly
		                        allocated memory.
		*/
		template <class T, class... Args>
		static UniqueHandle<T> Allocate(Args&&... oArgs);

		/*
		Attempts to allocate the provided amount of memory in the arena.

		@param uiCount - The number of elements to reserve memory for in the
		                 array.

		@return UniqueHandle<T> containing the handle that points to the newly
								allocated memory.
		*/
		template <class T>
		static UniqueHandle<T> AllocateArray(UInt32 uiCount);

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

		@return ByteCount containing the number of allocated bytes in this
		        MemoryManager.
		*/
		static ByteCount GetTotalAllocatedBytes();

		/*
		Returns the total number of bytes that are free in the MemoryManager.

		@return ByteCount containing the number of free bytes in this
		        MemoryManager.
		*/
		static ByteCount GetTotalFreeBytes();

		/*
		Prints a brief summary of the current memory usage.
		*/
		static void PrintMemory();

	private:
		MemoryManager() = delete;

		/*
		Returns the total number of allocated memory blocks (handles) in this
		MemoryManager.

		@return UInt32 containing the number of allocated memory blocks (handles)
		        in this MemoryManager.
		*/
		static UInt32 GetNodeCount();

		/*
		Creates a new handle pointing to a memory block that can hold the
		requested amount of memory. If this is not meant for an array, the
		element can be constructed in place with the passed arguments.

		@param uiCount - The number of elements to reserve space for at the
		                 new block of memory.

		@param oArgs - The arguments to be passed to the elements constructor
		               if this is not an array allocation.
		*/
		template <class T, class... Args>
		static Handle* SetupNewHandle(UInt32 uiCount, Args&&... oArgs);

		/*
		Deletes the provided handle and patches the handle table around it.

		@param opHandle - Pointer to the handle to be deleted.
		*/
		static void DeleteHandle(Handle* opHandle);

		/*
		Finds the first available memory block that can accomodate the
		requested byte size.

		@param uiRequestedSize - The number of bytes requested to be reserved.

		@param opPreviousHandleOut - The handle just before the new block.

		*/
		static void* FindFirstFreeMemoryBlock(ByteCount uiRequestedSize,
			Handle** opPreviousHandleOut);

		/*
		Moves the memory pointed to by the provided handle to the new location.

		@param opHandle - The handle whose memory needs to be moved.

		@param pNewLocation - Pointer to the new location to move the memory to.
		*/
		static void MoveHandle(Handle* opHandle, void* pNewLocation);
	
	private:
		static Byte* _suipMemoryStart; // Start of partitioned memory.
		static Byte* _suipAddressableMemoryStart; // Start of memory that can be allocated to.
		static Byte* _suipMemoryEnd; // End of addressable memory.
		static ByteCount _suiMemorySize; // Size of total reserved memory.
		static UInt32 _suiHandleTableLength; // Maximum amount of handles that can be created.
		static Handle* _sopHandleTableStart; // Start address of handle table.
		static Handle* _sopFirstHandle; // Address to the starting handle of the table.
		static bool _sbIsSetup; // Whether this MemoryManager has been initialized yet.
	};

	template <class T, class... Args>
	UniqueHandle<T> MemoryManager::Allocate(Args&&... oArgs)
	{
		Assert(_sbIsSetup);

		Handle* opNewHandle = SetupNewHandle<T>(1, std::forward<Args>(oArgs)...);
		UniqueHandle<T> oUniqueHandle(opNewHandle);
		return std::move(oUniqueHandle);
	}

	template <class T>
	UniqueHandle<T> MemoryManager::AllocateArray(UInt32 uiCount)
	{
		Assert(_sbIsSetup);

		Handle* opNewHandle = SetupNewHandle<T>(uiCount);
		UniqueHandle<T> oUniqueHandle(opNewHandle);
		return std::move(oUniqueHandle);
	}

	template <class T>
	void MemoryManager::Deallocate(Handle& oHandle)
	{
		Assert(_sbIsSetup);

		/*
		Destruct all elements at the given block
		*/
		T* pCurrentElement = (T*)oHandle.pLocation;

		for (UInt32 i = 0; i < oHandle.uiElementCount; ++i)
		{
			pCurrentElement->~T();
			++pCurrentElement;
		}

		DeleteHandle(&oHandle);
	}

	template <class T, class... Args>
	Handle* MemoryManager::SetupNewHandle(UInt32 uiCount, Args&&... oArgs)
	{
		// TODO: Move FindFirstFreeMemoryBlock call onto a separate thread.
		/*
		Find the first available memory slot that can accomodate this memory
		block.
		*/
		Handle* opPreviousHandle = nullptr;
		void* pAvailableBlock =
			FindFirstFreeMemoryBlock(uiCount * sizeof(T), &opPreviousHandle);

		/*
		Find the first available Handle.
		*/
		Handle* opCurrentHandle = _sopHandleTableStart;
		while (opCurrentHandle->bIsUsed)
		{
			++opCurrentHandle;
		}

		/*
		Allocate memory and configure handles. We only need to construct the
		object if we are not allocating for an array.
		*/
		if (uiCount == 1)
		{
			new (pAvailableBlock) T(std::forward<Args>(oArgs)...);
		}
		if (opPreviousHandle)
		{
			opCurrentHandle->opNextHandle = opPreviousHandle->opNextHandle;
			opPreviousHandle->opNextHandle = opCurrentHandle;
		}
		else
		{
			// This is the first handle of the table
			_sopFirstHandle = opCurrentHandle;
		}
		opCurrentHandle->pLocation = pAvailableBlock;
		opCurrentHandle->uiByteSize = uiCount * sizeof(T);
		opCurrentHandle->uiElementCount = uiCount;
		opCurrentHandle->bIsUsed = true;

		return opCurrentHandle;
	}
}