/*
Tests for the MemoryManager class.
@file MemoryManagerTests.h
@author Jacob Peterson
@edited 12/26/2020
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
		RunTest(VolatileAllocation);
	}

	bool MemoryManagerTests::BasicAllocation()
	{
		ByteCount uiBytes = MemoryManager::GetTotalAllocatedBytes();
		
		for (UInt8 i = 0; i < 255; ++i)
		{
			ByteCount uiBeforeAllocation = MemoryManager::GetTotalAllocatedBytes();
			UniqueHandle<UInt32> hIntHandle = MemoryManager::Allocate<UInt32>(3);
			AssertEqual(uiBeforeAllocation, MemoryManager::GetTotalAllocatedBytes() - 4,
				"Incorrect allocation of primitive.");
		}

		AssertEqual(uiBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Incorrect deallocation of arrays.");
	
		return true;
	}

	bool MemoryManagerTests::ArrayAllocation()
	{
		ByteCount uiBytes = MemoryManager::GetTotalAllocatedBytes();

		for (UInt8 i = 0; i < 255; ++i)
		{
			ByteCount uiBeforeAllocation = MemoryManager::GetTotalAllocatedBytes();
			UniqueHandle<UInt32> hIntHandle = MemoryManager::AllocateArray<UInt32>(1000);
			AssertEqual(uiBeforeAllocation, MemoryManager::GetTotalAllocatedBytes() - 4000,
				"Incorrect allocation of array.");
		}

		AssertEqual(uiBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Incorrect deallocation of arrays.");

		return true;
	}

	bool MemoryManagerTests::VolatileAllocation()
	{
		UInt32* uiInt = MemoryManager::AllocateVolatile<UInt32>();
		*uiInt = 50;

		AssertEqual(*uiInt, 50, "Failed to set Volatile memory.");

		MemoryManager::IncrementFrameCounter();

		UInt32* uiInt2 = MemoryManager::AllocateVolatile<UInt32>();

		AssertNotEqual(uiInt, uiInt2, "Cleared Volatile memory too early.");
		AssertEqual(uiInt + 1, uiInt2, "Incorrect allocation of Volatile memory.");

		MemoryManager::IncrementFrameCounter();

		uiInt2 = MemoryManager::AllocateVolatile<UInt32>();

		AssertEqual(uiInt, uiInt2, "Failed to clear Volatile memory.");

		return true;
	}
}