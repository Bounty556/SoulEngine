/*
Tests for the UniqueHandle class.
@file UniqueHandleTests.h
@author Jacob Peterson
@edited 12/21/2020
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
	}

	bool UniqueHandleTests::PrimitiveHandle()
	{
		AssertEqual(MemoryManager::GetTotalAllocatedBytes(), 0,
			"Initial memory condition not met.");

		{
			UniqueHandle<int> hInt = MemoryManager::Allocate<int>(0);

			AssertEqual(*hInt, 0, "Handle dereferencing failed.");

			UniqueHandle<int> hInt2 = MemoryManager::Allocate<int>(1);
			
			//hInt = std::move(hInt2);

			//AssertEqual(*hInt, 1, "Failed to move handle.");
		}

		AssertEqual(MemoryManager::GetTotalAllocatedBytes(), 0, 
			"Failed to deallocate primitives.");

		return true;
	}

	bool UniqueHandleTests::ObjectHandle()
	{
		AssertEqual(MemoryManager::GetTotalAllocatedBytes(), 0,
			"Initial memory condition not met.");

		TestClass oFake(1, 'u', 4.3f);
		TestClass oFake2(0, 'y', 9.6f);

		{
			UniqueHandle<TestClass> hFake = MemoryManager::Allocate<TestClass>(oFake);

			AssertEqual(*hFake, oFake, "Handle dereferencing failed.");

			UniqueHandle<TestClass> hFake2 = MemoryManager::Allocate<TestClass>(oFake2);

			hFake = std::move(hFake2);

			AssertEqual(*hFake, oFake2, "Failed to move handle.");
		}

		AssertEqual(MemoryManager::GetTotalAllocatedBytes(), 0,
			"Failed to deallocate objects.");

		return true;
	}

	bool UniqueHandleTests::HandleHandle()
	{
		AssertEqual(MemoryManager::GetTotalAllocatedBytes(), 0,
			"Initial memory condition not met.");

		{
			UniqueHandle<UniqueHandle<int>> hHandle =
				MemoryManager::Allocate<UniqueHandle<int>>(MemoryManager::Allocate<int>(0));

			AssertEqual(**hHandle, 0, "Handle dereferencing failed.");

			UniqueHandle<UniqueHandle<int>> hHandle2 =
				MemoryManager::Allocate<UniqueHandle<int>>(MemoryManager::Allocate<int>(1));

			hHandle = std::move(hHandle2);

			AssertEqual(**hHandle, 1, "Failed to move handle.");
		}

		AssertEqual(MemoryManager::GetTotalAllocatedBytes(), 0,
			"Failed to deallocate handles.");

		return true;
	}

	bool UniqueHandleTests::PrimitiveArrayHandle()
	{
		AssertEqual(MemoryManager::GetTotalAllocatedBytes(), 0,
			"Initial memory condition not met.");

		{
			UniqueHandle<int> hArray = MemoryManager::AllocateArray<int>(100);

			for (UInt8 i = 0; i < 100; ++i)
			{
				hArray[i] = i;
			}

			AssertEqual(hArray[0], 0, "Primitive array indexing failed.");

			UniqueHandle<int> hArray2 = MemoryManager::AllocateArray<int>(100);

			for (UInt8 i = 0; i < 100; ++i)
			{
				hArray2[i] = i + 1;
			}

			hArray = std::move(hArray2);

			AssertEqual(hArray[0], 1, "Primitive array moving failed.");
		}

		AssertEqual(MemoryManager::GetTotalAllocatedBytes(), 0,
			"Failed to deallocate primitive arrays.");

		return true;
	}

	bool UniqueHandleTests::ObjectArrayHandle()
	{
		AssertEqual(MemoryManager::GetTotalAllocatedBytes(), 0,
			"Initial memory condition not met.");

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

		AssertEqual(MemoryManager::GetTotalAllocatedBytes(), 0,
			"Failed to deallocate object arrays.");

		return true;
	}

	bool UniqueHandleTests::HandleArrayHandle()
	{
		AssertEqual(MemoryManager::GetTotalAllocatedBytes(), 0,
			"Initial memory condition not met.");

		{
			UniqueHandle<UniqueHandle<int>> hArray = MemoryManager::AllocateArray<UniqueHandle<int>>(100);

			for (UInt8 i = 0; i < 100; ++i)
			{
				hArray[i] = MemoryManager::Allocate<int>(0);
			}

			AssertEqual(*(hArray[0]), 0, "Handle array indexing failed.");

			UniqueHandle<UniqueHandle<int>> hArray2 = MemoryManager::AllocateArray<UniqueHandle<int>>(100);

			for (UInt8 i = 0; i < 100; ++i)
			{
				hArray2[i] = MemoryManager::Allocate<int>(1);
			}

			hArray = std::move(hArray2);

			AssertEqual(*(hArray[0]), 1, "Handle array moving failed.");
		}

		AssertEqual(MemoryManager::GetTotalAllocatedBytes(), 0,
			"Failed to deallocate handle arrays.");

		return true;
	}
}