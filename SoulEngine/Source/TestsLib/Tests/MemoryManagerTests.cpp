/*
Tests for the MemoryManager class.
@file MemoryManagerTests.h
@author Jacob Peterson
@version 1 12/21/2020
*/

#include "MemoryManagerTests.h"

#include <TestsLib/TestMacros.h>
#include <UtilsLib/CommonTypes.h>
#include <UtilsLib/Logger.h>
#include <UtilsLib/Memory/MemoryManager.h>
#include <UtilsLib/Memory/UniqueHandle.h>

namespace Soul
{
	void MemoryManagerTests::RunAllTests()
	{
		RunTest(BasicAllocation);
		RunTest(ArrayAllocation);
	}

	bool MemoryManagerTests::BasicAllocation()
	{
		AssertEqual(MemoryManager::GetTotalAllocatedBytes(), 0,
			"Initial memory condition not met.");
		
		for (UInt8 i = 0; i < 255; ++i)
		{
			MemoryManager::Allocate<int>(3);
			AssertEqual(MemoryManager::GetTotalAllocatedBytes(), 4,
				"Incorrect allocation of object.");
		}

		AssertEqual(MemoryManager::GetTotalAllocatedBytes(), 0,
			"Incorrect deallocation of objects.");
	
		return true;
	}

	bool MemoryManagerTests::ArrayAllocation()
	{
		AssertEqual(MemoryManager::GetTotalAllocatedBytes(), 0,
			"Initial memory condition not met.");

		for (UInt8 i = 0; i < 255; ++i)
		{
			MemoryManager::AllocateArray<int>(1000);
			AssertEqual(MemoryManager::GetTotalAllocatedBytes(), 4000,
				"Incorrect allocation of array.");
		}

		AssertEqual(MemoryManager::GetTotalAllocatedBytes(), 0,
			"Incorrect deallocation of arrays.");

		return true;
	}
}