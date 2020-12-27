/*
Tests for the UniqueHandle class.
@file UniqueHandleTests.cpp
@author Jacob Peterson
@edited 12/26/2020
*/

#include "UniqueHandleTests.h"

#include <TestsLib/TestMacros.h>
#include <UtilsLib/Memory/MemoryManager.h>
#include <UtilsLib/Memory/UniqueHandle.h>

namespace Soul
{
	void UniqueHandleTests::RunAllTests()
	{
		RunTest(PrimitiveHandle);
		RunTest(ObjectHandle);
		RunTest(HandleHandle);
		RunTest(PrimitiveArrayHandle);
		RunTest(ObjectArrayHandle);
		RunTest(HandleArrayHandle);
		RunTest(Detach);
		RunTest(Deallocate);
	}

	bool UniqueHandleTests::PrimitiveHandle()
	{
		ByteCount uiBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			UniqueHandle<UInt32> hInt = MemoryManager::Allocate<UInt32>(0);

			AssertEqual(*hInt, 0, "Handle dereferencing failed.");

			UniqueHandle<UInt32> hInt2 = MemoryManager::Allocate<UInt32>(1);
			
			hInt = std::move(hInt2);

			AssertEqual(*hInt, 1, "Failed to move handle.");
		}

		AssertEqual(uiBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate primitives.");

		return true;
	}

	bool UniqueHandleTests::ObjectHandle()
	{
		ByteCount uiBytes = MemoryManager::GetTotalAllocatedBytes();

		TestClass oFake(1, 'u', 4.3f);
		TestClass oFake2(0, 'y', 9.6f);

		{
			UniqueHandle<TestClass> hFake = MemoryManager::Allocate<TestClass>(oFake);

			AssertEqual(*hFake, oFake, "Handle dereferencing failed.");

			UniqueHandle<TestClass> hFake2 = MemoryManager::Allocate<TestClass>(oFake2);

			hFake = std::move(hFake2);

			AssertEqual(*hFake, oFake2, "Failed to move handle.");
		}

		AssertEqual(uiBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate objects.");

		return true;
	}

	bool UniqueHandleTests::HandleHandle()
	{
		ByteCount uiBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			UniqueHandle<UniqueHandle<UInt32>> hHandle =
				MemoryManager::Allocate<UniqueHandle<UInt32>>(MemoryManager::Allocate<UInt32>(0));

			AssertEqual(**hHandle, 0, "Handle dereferencing failed.");

			UniqueHandle<UniqueHandle<UInt32>> hHandle2 =
				MemoryManager::Allocate<UniqueHandle<UInt32>>(MemoryManager::Allocate<UInt32>(1));

			hHandle = std::move(hHandle2);

			AssertEqual(**hHandle, 1, "Failed to move handle.");
		}

		AssertEqual(uiBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate handles.");

		return true;
	}

	bool UniqueHandleTests::PrimitiveArrayHandle()
	{
		ByteCount uiBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			UniqueHandle<UInt32> hArray = MemoryManager::AllocateArray<UInt32>(100);

			for (UInt8 i = 0; i < 100; ++i)
			{
				hArray[i] = i;
			}

			AssertEqual(hArray[0], 0, "Primitive array indexing failed.");

			UniqueHandle<UInt32> hArray2 = MemoryManager::AllocateArray<UInt32>(100);

			for (UInt8 i = 0; i < 100; ++i)
			{
				hArray2[i] = i + 1;
			}

			hArray = std::move(hArray2);

			AssertEqual(hArray[0], 1, "Primitive array moving failed.");
		}

		AssertEqual(uiBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate primitive arrays.");

		return true;
	}

	bool UniqueHandleTests::ObjectArrayHandle()
	{
		ByteCount uiBytes = MemoryManager::GetTotalAllocatedBytes();

		TestClass oFake(1, 'u', 4.3f);
		TestClass oFake2(0, 'y', 9.6f);

		{
			UniqueHandle<TestClass> hArray = MemoryManager::AllocateArray<TestClass>(100);

			for (UInt8 i = 0; i < 100; ++i)
			{
				hArray[i] = oFake;
			}

			AssertEqual(hArray[0], oFake, "Object array indexing failed.");

			UniqueHandle<TestClass> hArray2 = MemoryManager::AllocateArray<TestClass>(100);

			for (UInt8 i = 0; i < 100; ++i)
			{
				hArray2[i] = oFake2;
			}

			hArray = std::move(hArray2);

			AssertEqual(hArray[0], oFake2, "Object array moving failed.");
		}

		AssertEqual(uiBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate object arrays.");

		return true;
	}

	bool UniqueHandleTests::HandleArrayHandle()
	{
		ByteCount uiBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			UniqueHandle<UniqueHandle<UInt32>> hArray = MemoryManager::AllocateArray<UniqueHandle<UInt32>>(100);

			for (UInt8 i = 0; i < 100; ++i)
			{
				hArray[i] = MemoryManager::Allocate<UInt32>(0);
			}

			AssertEqual(*(hArray[0]), 0, "Handle array indexing failed.");

			UniqueHandle<UniqueHandle<UInt32>> hArray2 = MemoryManager::AllocateArray<UniqueHandle<UInt32>>(100);

			for (UInt8 i = 0; i < 100; ++i)
			{
				hArray2[i] = MemoryManager::Allocate<UInt32>(1);
			}

			hArray = std::move(hArray2);

			AssertEqual(*(hArray[0]), 1, "Handle array moving failed.");
		}

		AssertEqual(uiBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate handle arrays.");

		return true;
	}

	bool UniqueHandleTests::Detach()
	{
		ByteCount uiBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			UniqueHandle<UInt32> hInt1 = MemoryManager::Allocate<UInt32>(1);
			UniqueHandle<UInt32> hInt2 = hInt1.Detach();

			AssertEqual(hInt1.IsValid(), false, "Failed to detach handle.");
			AssertEqual(*hInt2, 1, "Failed to reassign handle.");
		}

		AssertEqual(uiBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate detached handle.");

		return true;
	}

	bool UniqueHandleTests::Deallocate()
	{
		ByteCount uiBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			UniqueHandle<UInt32> hInt1 = MemoryManager::Allocate<UInt32>();

			AssertEqual(uiBytes, MemoryManager::GetTotalAllocatedBytes() - 4,
				"Failed to allocate handle.");

			hInt1.Deallocate();

			AssertEqual(uiBytes, MemoryManager::GetTotalAllocatedBytes(),
				"Failed to deallocate handle.");
		}

		return true;
	}
}