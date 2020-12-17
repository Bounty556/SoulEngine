#include "MemoryManager.h"

namespace Soul
{
	Byte* MemoryManager::_spMemoryStart;
	Byte* MemoryManager::_spMemoryEnd;
	ByteCount MemoryManager::_suiByteSize;

	// Getters /////////////////////////////////////////////////////////////////

	/*
	Takes the total memory size and subtracts the total free bytes from it.
	*/
	ByteCount MemoryManager::GetTotalAllocatedBytes()
	{
		return _suiByteSize - GetTotalFreeBytes();
	}

	/*
	Hops from MemoryNode to MemoryNode summing up the total free bytes.
	*/
	ByteCount MemoryManager::GetTotalFreeBytes()
	{
		MemoryNode* opCurrentNode = (MemoryNode*)_spMemoryStart;
		ByteCount uiTotalFreeBytes = 0;

		/*
		Traverse MemoryNodes.
		*/
		while (true)
		{
			uiTotalFreeBytes += opCurrentNode->uiBlockSize;
			if (opCurrentNode->opNextNode)
			{
				opCurrentNode = opCurrentNode->opNextNode;
			}
			else
			{
				break;
			}
		}
		
		return uiTotalFreeBytes;
	}
}