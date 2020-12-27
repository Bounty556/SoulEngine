/*
Reserves an initial amount of memory for the engine to be allocated as needed.
@file MemoryManager.cpp
@author Jacob Peterson
@edited 12/26/20
*/

#include "MemoryManager.h"

#include <UtilsLib/Logger.h>
#include <UtilsLib/Memory/UniqueHandle.h>

namespace Soul
{
	Byte* MemoryManager::_suipMemoryStart;
	Byte* MemoryManager::_suipAddressableMemoryStart;
	Byte* MemoryManager::_suipMemoryEnd;
	ByteCount MemoryManager::_suiMemorySize;
	UInt32 MemoryManager::_suiHandleTableLength = 512;
	Handle* MemoryManager::_sopHandleTableStart;
	Handle* MemoryManager::_sopFirstHandle;
	Byte* MemoryManager::_suipVolatileMemoryStart;
	Byte* MemoryManager::_suipVolatileMemoryEnd; 
	ByteCount MemoryManager::_suiVolatileMemorySize;
	Byte* MemoryManager::_suipVolatileNext;
	UInt8 MemoryManager::_suiFrameCounter;
	bool MemoryManager::_sbIsSetup = false;

	void MemoryManager::StartUp(ByteCount uiByteSize, ByteCount uiVolatileByteSize)
	{
		Assert(!_sbIsSetup);

		/*
		Allocate main memory.
		*/
		_suiMemorySize = uiByteSize - sizeof(Handle) * _suiHandleTableLength;
		_suipMemoryStart = (Byte*)malloc(_suiMemorySize);
		_suipMemoryEnd = _suipMemoryStart + _suiMemorySize;
		_sopHandleTableStart = (Handle*)_suipMemoryStart;
		_sopFirstHandle = nullptr;
		_suipAddressableMemoryStart = (Byte*)(_sopHandleTableStart + _suiHandleTableLength);

		/*
		Allocate volatile storage.
		*/
		_suiVolatileMemorySize = uiVolatileByteSize;
		_suipVolatileMemoryStart = (Byte*)malloc(_suiVolatileMemorySize);
		_suipVolatileMemoryEnd = _suipVolatileMemoryStart + _suiVolatileMemorySize;
		_suipVolatileNext = _suipVolatileMemoryStart;
		_suiFrameCounter = 0;

		_sbIsSetup = true;

		memset(_sopHandleTableStart, 0, sizeof(Handle) * _suiHandleTableLength);
		memset(_suipVolatileMemoryStart, 0, _suiVolatileMemorySize);
	}

	void MemoryManager::Shutdown()
	{
		Assert(_sbIsSetup);
		free(_suipMemoryStart);
		_suipMemoryStart = nullptr;
		_suipAddressableMemoryStart = nullptr;
		_suipMemoryEnd = nullptr;
		_suiMemorySize = 0;
		_sopHandleTableStart = nullptr; 
		_sopFirstHandle = nullptr;
		_sbIsSetup = false;
	}

	void MemoryManager::IncrementFrameCounter()
	{
		if (++_suiFrameCounter >= 2)
		{
			_suiFrameCounter = 0;
			memset(_suipVolatileMemoryStart, 0, _suiVolatileMemorySize);
			_suipVolatileNext = _suipVolatileMemoryStart;
		}
	}

	void MemoryManager::Defragment(UInt8 uiBlockCount)
	{
		Assert(_sbIsSetup);

		/*
		Find the first N gaps, move the memory blocks over to fill the gaps.
		*/
		Byte* uipPreviousBlockEnd = _suipAddressableMemoryStart;
		Handle* opCurrentHandle = _sopFirstHandle;
		UInt8 uiMovedBlocks = 0;
		while (opCurrentHandle && uiMovedBlocks < uiBlockCount)
		{
			ByteCount uiDistance =
				ByteDistance(uipPreviousBlockEnd, opCurrentHandle->pLocation);
			if (uiDistance > 0)
			{
				MoveHandle(opCurrentHandle, uipPreviousBlockEnd);
				++uiMovedBlocks;
			}

			uipPreviousBlockEnd =
				(Byte*)opCurrentHandle->pLocation + opCurrentHandle->uiByteSize;
			opCurrentHandle = opCurrentHandle->opNextHandle;
		}
	}

	ByteCount MemoryManager::GetTotalAllocatedBytes()
	{
		Assert(_sbIsSetup);

		ByteCount uiTotalBytes = 0;
		Handle* opCurrentHandle = _sopFirstHandle;
		while (opCurrentHandle)
		{
			uiTotalBytes += opCurrentHandle->uiByteSize;
			opCurrentHandle = opCurrentHandle->opNextHandle;
		}
		return uiTotalBytes;
	}

	ByteCount MemoryManager::GetTotalFreeBytes()
	{
		Assert(_sbIsSetup);
		return _suiMemorySize - GetTotalAllocatedBytes();
	}

	void MemoryManager::PrintMemory()
	{
		Assert(_sbIsSetup);

		SoulLogInfo("\n\tNodes: %d\n\tFree Bytes: %lld\n\tAllocated Bytes: %lld\n\tFragments: %d", GetNodeCount(), GetTotalFreeBytes(), GetTotalAllocatedBytes(), CountFragments());
	}

	HandleTableSize MemoryManager::GetNodeCount()
	{
		Handle* opCurrentHandle = _sopFirstHandle;
		UInt32 uiHandleCount = 0;
		while (opCurrentHandle)
		{
			++uiHandleCount;
			opCurrentHandle = opCurrentHandle->opNextHandle;
		}
		return uiHandleCount;
	}

	HandleTableSize MemoryManager::CountFragments()
	{
		/*
		Find the memory gaps.
		*/
		Byte* uipPreviousBlockEnd = _suipAddressableMemoryStart;
		Handle* opCurrentHandle = _sopFirstHandle;
		HandleTableSize uiMemoryFragments = 0;
		while (opCurrentHandle)
		{
			ByteCount uiDistance =
				ByteDistance(uipPreviousBlockEnd, opCurrentHandle->pLocation);
			if (uiDistance > 0)
			{
				++uiMemoryFragments;
			}

			uipPreviousBlockEnd =
				(Byte*)opCurrentHandle->pLocation + opCurrentHandle->uiByteSize;
			opCurrentHandle = opCurrentHandle->opNextHandle;
		}

		return uiMemoryFragments;
	}

	void MemoryManager::DeleteHandle(Handle* opHandle)
	{
		if (opHandle == _sopFirstHandle)
		{
			_sopFirstHandle = opHandle->opNextHandle;
		}
		else
		{
			/*
			Find the handle just before the provided handle.
			*/
			Handle* opCurrentHandle = _sopFirstHandle;

			while (opCurrentHandle->opNextHandle != opHandle)
			{
				opCurrentHandle = opCurrentHandle->opNextHandle;
			}

			/*
			Patch the list around the removed handle.
			*/
			opCurrentHandle->opNextHandle = opCurrentHandle->opNextHandle->opNextHandle;
		}
		
		/*
		Free the handle
		*/
		memset(opHandle, 0, sizeof(Handle));
	}

	void* MemoryManager::FindFirstFreeMemoryBlock(ByteCount uiRequestedSize,
		Handle** opPreviousHandleOut)
	{
		/*
		If there are no handles yet, don't go through the process.
		*/
		if (!_sopFirstHandle)
		{
			return _suipAddressableMemoryStart;
		}

		Handle* opCurrentHandle = _sopFirstHandle;
		Handle* opNextHandle = opCurrentHandle->opNextHandle;

		while (opNextHandle)
		{
			/*
			Check the space between this handle and the next
			*/
			Byte* uipEndOfBlock =
				(Byte*)opCurrentHandle->pLocation + opCurrentHandle->uiByteSize;
			if (ByteDistance(uipEndOfBlock, opNextHandle->pLocation) >= uiRequestedSize)
			{
				(*opPreviousHandleOut) = opCurrentHandle;
				return uipEndOfBlock;
			}
			else
			{
				opCurrentHandle = opNextHandle;
				opNextHandle = opNextHandle->opNextHandle;
			}
		}

		/*
		There must be space between this last handle and the end of the
		allocated memory, otherwise we have run out of memory and we will crash.
		*/
		Byte* uipEndOfBlock =
			(Byte*)opCurrentHandle->pLocation + opCurrentHandle->uiByteSize;
		if (ByteDistance(uipEndOfBlock, _suipMemoryEnd) >= uiRequestedSize)
		{
			(*opPreviousHandleOut) = opCurrentHandle;
			return uipEndOfBlock;
		}
		else
		{
			SoulLogError("Ran out of memory.");
			Assert(false);
			return nullptr;
		}
	}

	void MemoryManager::MoveHandle(Handle* opHandle, void* pNewLocation)
	{
		memcpy(pNewLocation, opHandle->pLocation, opHandle->uiByteSize);
		opHandle->pLocation = pNewLocation;
	}
}