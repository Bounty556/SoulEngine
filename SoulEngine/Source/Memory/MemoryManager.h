/*
Reserves an initial amount of memory for the engine to be allocated as needed.
@file MemoryManager.h
@author Jacob Peterson
@edited 4/18/21
*/

#pragma once

#include <memory>
#include <new>
#include <utility>

#include <UtilsLib/CommonTypes.h>
#include <UtilsLib/Macros.h>

typedef PtrSize ByteCount;
typedef UInt32 HandleTableSize;

namespace Soul
{
	template <class T>
	class UniqueHandle;

	/*
	Returned when allocating memory. Should be used in a UniqueHandle object.
	*/
	struct Handle
	{
		Handle* nextHandle; // The handle closest to this one.
		void* location; // Location that this handle points to in the memory arena.
		ByteCount byteSize; // Size of the memory block that this handle points to.
		ArraySize elementCount; // Number of elements allocated in the memory block.
		bool isUsed; // Whether this handle is currently in use.
		bool isCopyable; // Whether the data under this handle can be trivially copied.
	};

	/*
	A singleton MemoryManager for the Soul engine. This first needs to be
	initialized by calling StartUp() (usually done by the engine) and cleaned up
	via Shutdown().

	To partition memory for use, call Allocate<T>(...) or AllocateArray<T> with
	the desired type inserted into the template. This will return a new
	UniqueHandle which can then be used to access the memory block that was
	allocated for the object.

	For debugging purposes, the GetTotalAllocatedBytes(), GetTotalFreeBytes(),
	and CountFragments() functions can be used to query the current usage of
	the memory arena. The PrintMemory() function also prints out a brief summary
	of the current memory usage.
	*/
	class MemoryManager
	{
	public:

		/*
		Initializes the MemoryManager's memory and sets up the Handle table.

		@param byteSize - The number of bytes to reserve for this MemoryManager.

		@param volatileByteSize - The number of bytes to reserve for the
		                            the volatile memory storage.
		*/
		static void StartUp(ByteCount byteSize, ByteCount volatileByteSize);

		/*
		Shuts down the MemoryManager and frees all its memory.
		*/
		static void Shutdown();

		/*
		Attempts to allocate memory for the provided object type.

		@param args - The arguments to initialize the object with.

		@return UniqueHandle<T> containing the handle that points to the newly
		                        allocated memory.
		*/
		template <class T, class... Args>
		static UniqueHandle<T> Allocate(Args&&... args);

		/*
		Attempts to allocate the provided amount of memory in the arena.

		@param count - The number of elements to reserve memory for in the
		                 array.

		@return UniqueHandle<T> containing the handle that points to the newly
								allocated memory.
		*/
		template <class T>
		static UniqueHandle<T> AllocateArray(ArraySize count);

		/*
		Allocates memory in the volatile memory arena. This memory gets cleared
		every other frame.

		@param count - Number of elements to allocate.

		@return Pointer to the newly allocated memory.
		*/
		template <class T>
		static T* AllocateVolatile(ArraySize count = 1);

		/*
		Calls the destructor and frees the memory for every object allocated to
		the provided handle.

		@param handle - The handle whose memory needs to be freed.
		*/
		template <class T>
		static void Deallocate(Handle& handle);

		/*
		Increases the frame memory counter. Once the frame memory counter
		reaches two, the memory is cleared.
		*/
		static void IncrementFrameCounter();

		/*
		Attempts to defragment the provided number of blocks to keep memory
		contiguous and cache-friendly.

		@param blockCount - The number of memory blocks to attempt to defrag.
		*/
		static void Defragment(UInt8 blockCount);

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

		/*
		Counts the total amount of empty blocks of memory that are between the
		start of the addressable memory and the last block of memory.

		@return The number of empty block fragments.
		*/
		static HandleTableSize CountFragments();

	private:
		MemoryManager() = delete;

		/*
		Returns the total number of allocated memory blocks (handles) in this
		MemoryManager.

		@return UInt32 containing the number of allocated memory blocks (handles)
		        in this MemoryManager.
		*/
		static HandleTableSize GetNodeCount();

		/*
		Creates a new handle pointing to a memory block that can hold the
		requested amount of memory. If this is not meant for an array, the
		element can be constructed in place with the passed arguments.

		@param count - The number of elements to reserve space for at the
		                 new block of memory.
		*/
		template <class T>
		static Handle* SetupNewHandle(ArraySize count);

		/*
		Deletes the provided handle and patches the handle table around it.

		@param handle - Pointer to the handle to be deleted.
		*/
		static void DeleteHandle(Handle* handle);

		/*
		Finds the first available memory block that can accomodate the
		requested byte size.

		@param requestedSize - The number of bytes requested to be reserved.

		@param previousHandleOut - The handle just before the new block.

		*/
		static void* FindFirstFreeMemoryBlock(ByteCount requestedSize,
			Handle** previousHandleOut);

		/*
		Moves the memory pointed to by the provided handle to the new location.

		@param handle - The handle whose memory needs to be moved.

		@param newLocation - Pointer to the new location to move the memory to.
		*/
		static void MoveHandle(Handle* handle, void* newLocation);
	
	private:
		static Byte* m_MemoryStart; // Start of partitioned memory.
		static Byte* m_AddressableMemoryStart; // Start of memory that can be allocated to.
		static Byte* m_MemoryEnd; // End of addressable memory.
		static ByteCount m_MemorySize; // Size of total reserved memory.
		static HandleTableSize m_HandleTableLength; // Maximum amount of handles that can be created.
		static Handle* m_HandleTableStart; // Start address of handle table.
		static Handle* m_FirstHandle; // Address to the starting handle of the table.

		static Byte* m_VolatileMemoryStart; // Start of volatile partitioned memory.
		static Byte* m_VolatileMemoryEnd; // End of volatile partitioned memory.
		static ByteCount m_VolatileMemorySize; // Size of volatile memory.
		static Byte* m_VolatileNext; // The next address to allocate volatile memory to.
		static UInt8 m_FrameCounter; // Number of frames since last volatile memory clear.

		static bool m_IsSetup; // Whether this MemoryManager has been initialized yet.
	};

	template <class T, class... Args>
	UniqueHandle<T> MemoryManager::Allocate(Args&&... args)
	{
		Assert(m_IsSetup);

		Handle* newHandle = SetupNewHandle<T>(1);
		new (newHandle->location) T(std::forward<Args>(args)...);
		UniqueHandle<T> uniqueHandle(newHandle);
		return std::move(uniqueHandle);
	}

	template <class T>
	UniqueHandle<T> MemoryManager::AllocateArray(ArraySize count)
	{
		Assert(m_IsSetup);

		Handle* newHandle = SetupNewHandle<T>(count);
		UniqueHandle<T> uniqueHandle(newHandle);
		return std::move(uniqueHandle);
	}

	template <class T>
	static T* MemoryManager::AllocateVolatile(ArraySize count /*=1*/)
	{
		Assert(m_VolatileNext + count * sizeof(T) <= m_VolatileMemoryEnd);

		T* memory = (T*)m_VolatileNext;
		m_VolatileNext += count * sizeof(T);

		return memory;
	}

	template <class T>
	void MemoryManager::Deallocate(Handle& handle)
	{
		Assert(m_IsSetup);

		/*
		Destruct all elements at the given block
		*/
		T* currentElement = (T*)handle.location;
		for (UInt32 i = 0; i < handle.elementCount; ++i)
		{
			currentElement->~T();
			++currentElement;
		}

		DeleteHandle(&handle);
	}

	template <class T>
	Handle* MemoryManager::SetupNewHandle(ArraySize count)
	{
		// TODO: Move FindFirstFreeMemoryBlock call onto a separate thread.
		/*
		Find the first available memory slot that can accomodate this memory
		block.
		*/
		Handle* previousHandle = nullptr;
		void* availableBlock =
			FindFirstFreeMemoryBlock(count * sizeof(T), &previousHandle);

		/*
		Find the first available Handle.
		*/
		Handle* currentHandle = m_HandleTableStart;
		while (currentHandle->isUsed)
		{
			++currentHandle;
		}

		/*
		Allocate memory and configure handles. We only need to construct the
		object if we are not allocating for an array. If we're not allocating
		an array, just set the memory to 0.
		*/
		memset(availableBlock, 0, count * sizeof(T));
		if (previousHandle)
		{
			currentHandle->nextHandle = previousHandle->nextHandle;
			previousHandle->nextHandle = currentHandle;
		}
		else
		{
			// This is the first handle of the table
			m_FirstHandle = currentHandle;
		}
		currentHandle->location = availableBlock;
		currentHandle->byteSize = count * sizeof(T);
		currentHandle->elementCount = count;
		currentHandle->isUsed = true;
		currentHandle->isCopyable = true;

		return currentHandle;
	}
}