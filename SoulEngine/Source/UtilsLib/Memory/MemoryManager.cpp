/*
Reserves an initial amount of memory for the engine to be allocated as needed.
@file MemoryManager.cpp
@author Jacob Peterson
@version 3 12/17/20
*/

#include "MemoryManager.h"

#include <UtilsLib/Memory/UniqueHandle.h>

#include <memory>

#include <UtilsLib/Logger.h>

namespace Soul
{
	Byte* MemoryManager::_suipMemoryStart;
	Byte* MemoryManager::_suipAddressableMemoryStart;
	Byte* MemoryManager::_suipMemoryEnd;
	ByteCount MemoryManager::_suiMemorySize;
	UInt32 MemoryManager::_suiHandleTableLength = 256;
	Handle* MemoryManager::_sopHandleTableStart;
	Handle* MemoryManager::_sopFirstHandle;
	bool MemoryManager::_sbIsSetup = false;

	void MemoryManager::StartUp(ByteCount uiByteSize)
	{
		Assert(!_sbIsSetup);

		/*
		Allocate the necessary memory
		*/
		_suiMemorySize = uiByteSize - sizeof(Handle) * _suiHandleTableLength;
		_suipMemoryStart = (Byte*)malloc(_suiMemorySize);
		_suipMemoryEnd = _suipMemoryStart + _suiMemorySize;
		_sopHandleTableStart = (Handle*)_suipMemoryStart;
		_sopFirstHandle = nullptr;
		_suipAddressableMemoryStart = (Byte*)(_sopHandleTableStart + _suiHandleTableLength);
		_sbIsSetup = true;

		memset(_sopHandleTableStart, 0, sizeof(Handle) * _suiHandleTableLength);
	}

	void MemoryManager::Shutdown()
	{
		Assert(_sbIsSetup);
		free(_suipMemoryStart);
	}

	void MemoryManager::Defragment(UInt8 uiBlockCount)
	{
		Assert(_sbIsSetup);

		// TODO: Implement
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
		SoulLogInfo("\n\tNodes: %d\n\tFree Bytes: %lld\n\tAllocated Bytes: %lld",
			        GetNodeCount(), GetTotalFreeBytes(), GetTotalAllocatedBytes());
	}

	UInt32 MemoryManager::GetNodeCount()
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

	void MemoryManager::DeleteHandle(Handle* opHandle)
	{
		if (opHandle == _sopFirstHandle)
		{
			_sopFirstHandle = nullptr;
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
				(Byte*)opCurrentHandle->uipLocation + opCurrentHandle->uiByteSize;
			if (ByteDistance(uipEndOfBlock, opNextHandle->uipLocation) >= uiRequestedSize)
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
			(Byte*)opCurrentHandle->uipLocation + opCurrentHandle->uiByteSize;
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
}