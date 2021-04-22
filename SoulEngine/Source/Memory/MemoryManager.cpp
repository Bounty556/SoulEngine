/*
Reserves an initial amount of memory for the engine to be allocated as needed.
@file MemoryManager.cpp
@author Jacob Peterson
@edited 4/21/21
*/

#include "MemoryManager.h"

#include <Memory/UniqueHandle.h>
#include <UtilsLib/Logger.h>

namespace Soul
{
	Byte* MemoryManager::m_MemoryStart;
	Byte* MemoryManager::m_AddressableMemoryStart;
	Byte* MemoryManager::m_MemoryEnd;
	ByteCount MemoryManager::m_MemorySize;
	UInt32 MemoryManager::m_HandleTableLength = 512;
	Handle* MemoryManager::m_HandleTableStart;
	Handle* MemoryManager::m_FirstHandle;
	Byte* MemoryManager::m_VolatileMemoryStart;
	Byte* MemoryManager::m_VolatileMemoryEnd; 
	ByteCount MemoryManager::m_VolatileMemorySize;
	Byte* MemoryManager::m_VolatileNext;
	UInt8 MemoryManager::m_FrameCounter;
	bool MemoryManager::m_IsSetup = false;

	void MemoryManager::StartUp(ByteCount byteSize, ByteCount volatileByteSize)
	{
		Assert(!m_IsSetup);

		/*
		Allocate main memory.
		*/
		m_MemorySize = byteSize - sizeof(Handle) * m_HandleTableLength;
		m_MemoryStart = (Byte*)malloc(m_MemorySize);
		m_MemoryEnd = m_MemoryStart + m_MemorySize;
		m_HandleTableStart = (Handle*)m_MemoryStart;
		m_FirstHandle = nullptr;
		m_AddressableMemoryStart = (Byte*)(m_HandleTableStart + m_HandleTableLength);

		/*
		Allocate volatile storage.
		*/
		m_VolatileMemorySize = volatileByteSize;
		m_VolatileMemoryStart = (Byte*)malloc(m_VolatileMemorySize);
		m_VolatileMemoryEnd = m_VolatileMemoryStart + m_VolatileMemorySize;
		m_VolatileNext = m_VolatileMemoryStart;
		m_FrameCounter = 0;

		m_IsSetup = true;

		memset(m_HandleTableStart, 0, sizeof(Handle) * m_HandleTableLength);
		memset(m_VolatileMemoryStart, 0, m_VolatileMemorySize);
	}

	void MemoryManager::Shutdown()
	{
		Assert(m_IsSetup);
		free(m_MemoryStart);
		m_MemoryStart = nullptr;
		m_AddressableMemoryStart = nullptr;
		m_MemoryEnd = nullptr;
		m_MemorySize = 0;
		m_HandleTableStart = nullptr; 
		m_FirstHandle = nullptr;
		m_IsSetup = false;
	}

	void MemoryManager::IncrementFrameCounter()
	{
		if (++m_FrameCounter >= 2)
		{
			m_FrameCounter = 0;
			memset(m_VolatileMemoryStart, 0, m_VolatileMemorySize);
			m_VolatileNext = m_VolatileMemoryStart;
		}
	}

	void MemoryManager::Defragment(UInt8 blockCount)
	{
		Assert(m_IsSetup);

		/*
		Find the first N gaps, move the memory blocks over to fill the gaps.
		*/
		Byte* previousBlockEnd = m_AddressableMemoryStart;
		Handle* currentHandle = m_FirstHandle;
		UInt8 movedBlocks = 0;
		while (currentHandle && movedBlocks < blockCount)
		{
			ByteCount uiDistance =
				ByteDistance(previousBlockEnd, currentHandle->location);

			// Only defrag this block if it is movable and can be moved.
			if (uiDistance > 0 && currentHandle->isCopyable)
			{
				MoveHandle(currentHandle, previousBlockEnd);
				++movedBlocks;
			}

			previousBlockEnd =
				(Byte*)currentHandle->location + currentHandle->byteSize;
			currentHandle = currentHandle->nextHandle;
		}
	}

	ByteCount MemoryManager::GetTotalAllocatedBytes()
	{
		Assert(m_IsSetup);

		ByteCount totalBytes = 0;
		Handle* currentHandle = m_FirstHandle;
		while (currentHandle)
		{
			totalBytes += currentHandle->byteSize;
			currentHandle = currentHandle->nextHandle;
		}
		return totalBytes;
	}

	ByteCount MemoryManager::GetTotalFreeBytes()
	{
		Assert(m_IsSetup);
		return m_MemorySize - GetTotalAllocatedBytes();
	}

	void MemoryManager::PrintMemory()
	{
		Assert(m_IsSetup);

		SoulLogInfo("\n\tNodes: %d\n\tFree Bytes: %lld\n\tAllocated Bytes: %lld\n\tFragments: %d", GetNodeCount(), GetTotalFreeBytes(), GetTotalAllocatedBytes(), CountFragments());
	}

	HandleTableSize MemoryManager::CountFragments()
	{
		/*
		Find the memory gaps.
		*/
		Byte* previousBlockEnd = m_AddressableMemoryStart;
		Handle* currentHandle = m_FirstHandle;
		HandleTableSize memoryFragments = 0;
		while (currentHandle)
		{
			ByteCount uiDistance =
				ByteDistance(previousBlockEnd, currentHandle->location);
			if (uiDistance > 0)
			{
				++memoryFragments;
			}

			previousBlockEnd =
				(Byte*)currentHandle->location + currentHandle->byteSize;
			currentHandle = currentHandle->nextHandle;
		}

		return memoryFragments;
	}

	HandleTableSize MemoryManager::GetNodeCount()
	{
		Handle* currentHandle = m_FirstHandle;
		UInt32 handleCount = 0;
		while (currentHandle)
		{
			++handleCount;
			currentHandle = currentHandle->nextHandle;
		}
		return handleCount;
	}

	void MemoryManager::DeleteHandle(Handle* handlePointer)
	{
		if (handlePointer == m_FirstHandle)
		{
			m_FirstHandle = handlePointer->nextHandle;
		}
		else
		{
			/*
			Find the handle just before the provided handle.
			*/
			Handle* currentHandle = m_FirstHandle;

			while (currentHandle->nextHandle != handlePointer)
			{
				currentHandle = currentHandle->nextHandle;
			}

			/*
			Patch the list around the removed handle.
			*/
			currentHandle->nextHandle = currentHandle->nextHandle->nextHandle;
		}
		
		/*
		Free the handle
		*/
		memset(handlePointer, 0, sizeof(Handle));
	}

	void* MemoryManager::FindFirstFreeMemoryBlock(ByteCount requestedSize,
		Handle** previousHandleOut)
	{
		/*
		If there are no handles yet, don't go through the process.
		*/
		if (!m_FirstHandle)
		{
			return m_AddressableMemoryStart;
		}

		Handle* currentHandle = m_FirstHandle;
		Handle* nextHandle = currentHandle->nextHandle;

		while (nextHandle)
		{
			/*
			Check the space between this handle and the next
			*/
			Byte* endOfBlock =
				(Byte*)currentHandle->location + currentHandle->byteSize;
			if (ByteDistance(endOfBlock, nextHandle->location) >= requestedSize)
			{
				(*previousHandleOut) = currentHandle;
				return endOfBlock;
			}
			else
			{
				currentHandle = nextHandle;
				nextHandle = nextHandle->nextHandle;
			}
		}

		/*
		There must be space between this last handle and the end of the
		allocated memory, otherwise we have run out of memory and we will crash.
		*/
		Byte* endOfBlock =
			(Byte*)currentHandle->location + currentHandle->byteSize;
		if (ByteDistance(endOfBlock, m_MemoryEnd) >= requestedSize)
		{
			(*previousHandleOut) = currentHandle;
			return endOfBlock;
		}
		else
		{
			SoulLogError("Ran out of memory.");
			Assert(false);
			return nullptr;
		}
	}

	void MemoryManager::MoveHandle(Handle* handle, void* newLocation)
	{
		Assert(handle->isCopyable);

		memcpy(newLocation, handle->location, handle->byteSize);
		handle->location = newLocation;
	}
}