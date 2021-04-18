/*
Tests for the MemoryManager class.
@file MemoryManagerTests.h
@author Jacob Peterson
@edited 4/18/21
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
		RunTest(ImmovableAllocation);
		RunTest(MemoryDefragmentation);
	}

	bool MemoryManagerTests::BasicAllocation()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();
		
		for (UInt8 i = 0; i < 255; ++i)
		{
			ByteCount bytesBeforeAlloc = MemoryManager::GetTotalAllocatedBytes();
			UniqueHandle<UInt32> uniqueInt = MemoryManager::Allocate<UInt32>(3);
			AssertEqual(bytesBeforeAlloc, MemoryManager::GetTotalAllocatedBytes() - 4,
				"Incorrect allocation of primitive.");
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Incorrect deallocation of arrays.");
	
		return true;
	}

	bool MemoryManagerTests::ArrayAllocation()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		for (UInt8 i = 0; i < 255; ++i)
		{
			ByteCount bytesBeforeAlloc = MemoryManager::GetTotalAllocatedBytes();
			UniqueHandle<UInt32> uniqueIntArray = MemoryManager::AllocateArray<UInt32>(1000);
			AssertEqual(bytesBeforeAlloc, MemoryManager::GetTotalAllocatedBytes() - 4000,
				"Incorrect allocation of array.");
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Incorrect deallocation of arrays.");

		return true;
	}

	bool MemoryManagerTests::VolatileAllocation()
	{
		UInt32* volatileInt = MemoryManager::AllocateVolatile<UInt32>();
		*volatileInt = 50;

		AssertEqual(*volatileInt, 50, "Failed to set Volatile memory.");

		MemoryManager::IncrementFrameCounter();

		UInt32* volatileInt2 = MemoryManager::AllocateVolatile<UInt32>();

		AssertNotEqual(volatileInt, volatileInt2, "Cleared Volatile memory too early.");
		AssertEqual(volatileInt + 1, volatileInt2, "Incorrect allocation of Volatile memory.");

		MemoryManager::IncrementFrameCounter();

		volatileInt2 = MemoryManager::AllocateVolatile<UInt32>();

		AssertEqual(volatileInt, volatileInt2, "Failed to clear Volatile memory.");

		return true;
	}

	bool MemoryManagerTests::ImmovableAllocation()
	{
		UniqueHandle<Int8> uniqueInt1;

		{
			UniqueHandle<Int8> uniqueInt2 = MemoryManager::Allocate<Int8>(1);
			UniqueHandle<Int8> uniqueInt3 = MemoryManager::Allocate<Int8>(1);

			uniqueInt1 = MemoryManager::Allocate<Int8>(1);
			uniqueInt1.SetImmovable(true);
		}

		AssertEqual(MemoryManager::CountFragments(), 1, "Incorrect deallocation of data.");

		MemoryManager::Defragment(1);

		AssertEqual(MemoryManager::CountFragments(), 1, "Immovable data moved.");

		return true;
	}

	bool MemoryManagerTests::MemoryDefragmentation()
	{
		UniqueHandle<Int8> uniqueInt1;
		
		{
			UniqueHandle<Int8> uniqueInt2 = MemoryManager::Allocate<Int8>(1);
			UniqueHandle<Int8> uniqueInt3 = MemoryManager::Allocate<Int8>(1);

			uniqueInt1 = MemoryManager::Allocate<Int8>(1);
		}
		
		AssertEqual(MemoryManager::CountFragments(), 1, "Incorrect deallocation of data.");

		MemoryManager::Defragment(1);

		AssertEqual(MemoryManager::CountFragments(), 0, "Defragmentation failed.");

		return true;

		return true;
	}
}