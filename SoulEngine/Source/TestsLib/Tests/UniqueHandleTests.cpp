/*
Tests for the UniqueHandle class.
@file UniqueHandleTests.cpp
@author Jacob Peterson
@edited 4/21/21
*/

#include "UniqueHandleTests.h"

#include <Memory/MemoryManager.h>
#include <Memory/UniqueHandle.h>
#include <TestsLib/TestMacros.h>

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
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			UniqueHandle<UInt32> uniqueInt = MemoryManager::Allocate<UInt32>(0);

			AssertEqual(*uniqueInt, 0, "Handle dereferencing failed.");

			UniqueHandle<UInt32> uniqueInt2 = MemoryManager::Allocate<UInt32>(1);
			
			uniqueInt = std::move(uniqueInt2);

			AssertEqual(*uniqueInt, 1, "Failed to move handle.");
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate primitives.");

		return true;
	}

	bool UniqueHandleTests::ObjectHandle()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		TestClass fakeClass(1, 'u', 4.3f);
		TestClass fakeClass2(0, 'y', 9.6f);

		{
			UniqueHandle<TestClass> uniqueClass = MemoryManager::Allocate<TestClass>(fakeClass);

			AssertEqual(*uniqueClass, fakeClass, "Handle dereferencing failed.");

			UniqueHandle<TestClass> uniqueClass2 = MemoryManager::Allocate<TestClass>(fakeClass2);

			uniqueClass = std::move(uniqueClass2);

			AssertEqual(*uniqueClass, fakeClass2, "Failed to move handle.");
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate objects.");

		return true;
	}

	bool UniqueHandleTests::HandleHandle()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			UniqueHandle<UniqueHandle<UInt32>> handleToUniqueInt =
				MemoryManager::Allocate<UniqueHandle<UInt32>>(MemoryManager::Allocate<UInt32>(0));

			AssertEqual(**handleToUniqueInt, 0, "Handle dereferencing failed.");

			UniqueHandle<UniqueHandle<UInt32>> handleToUniqueInt2 =
				MemoryManager::Allocate<UniqueHandle<UInt32>>(MemoryManager::Allocate<UInt32>(1));

			handleToUniqueInt = std::move(handleToUniqueInt2);

			AssertEqual(**handleToUniqueInt, 1, "Failed to move handle.");
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate handles.");

		return true;
	}

	bool UniqueHandleTests::PrimitiveArrayHandle()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			UniqueHandle<UInt32> uniqueArray = MemoryManager::AllocateArray<UInt32>(100);

			for (UInt8 i = 0; i < 100; ++i)
			{
				uniqueArray[i] = i;
			}

			AssertEqual(uniqueArray[0], 0, "Primitive array indexing failed.");

			UniqueHandle<UInt32> uniqueArray2 = MemoryManager::AllocateArray<UInt32>(100);

			for (UInt8 i = 0; i < 100; ++i)
			{
				uniqueArray2[i] = i + 1;
			}

			uniqueArray = std::move(uniqueArray2);

			AssertEqual(uniqueArray[0], 1, "Primitive array moving failed.");
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate primitive arrays.");

		return true;
	}

	bool UniqueHandleTests::ObjectArrayHandle()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		TestClass fakeClass(1, 'u', 4.3f);
		TestClass fakeClass2(0, 'y', 9.6f);

		{
			UniqueHandle<TestClass> uniqueArray = MemoryManager::AllocateArray<TestClass>(100);

			for (UInt8 i = 0; i < 100; ++i)
			{
				uniqueArray[i] = fakeClass;
			}

			AssertEqual(uniqueArray[0], fakeClass, "Object array indexing failed.");

			UniqueHandle<TestClass> uniqueArray2 = MemoryManager::AllocateArray<TestClass>(100);

			for (UInt8 i = 0; i < 100; ++i)
			{
				uniqueArray2[i] = fakeClass2;
			}

			uniqueArray = std::move(uniqueArray2);

			AssertEqual(uniqueArray[0], fakeClass2, "Object array moving failed.");
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate object arrays.");

		return true;
	}

	bool UniqueHandleTests::HandleArrayHandle()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			UniqueHandle<UniqueHandle<UInt32>> handleToUniqueArray = MemoryManager::AllocateArray<UniqueHandle<UInt32>>(100);

			for (UInt8 i = 0; i < 100; ++i)
			{
				handleToUniqueArray[i] = MemoryManager::Allocate<UInt32>(0);
			}

			AssertEqual(*(handleToUniqueArray[0]), 0, "Handle array indexing failed.");

			UniqueHandle<UniqueHandle<UInt32>> handleToUniqueArray2 = MemoryManager::AllocateArray<UniqueHandle<UInt32>>(100);

			for (UInt8 i = 0; i < 100; ++i)
			{
				handleToUniqueArray2[i] = MemoryManager::Allocate<UInt32>(1);
			}

			handleToUniqueArray = std::move(handleToUniqueArray2);

			AssertEqual(*(handleToUniqueArray[0]), 1, "Handle array moving failed.");
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate handle arrays.");

		return true;
	}

	bool UniqueHandleTests::Detach()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			UniqueHandle<UInt32> uniqueInt = MemoryManager::Allocate<UInt32>(1);
			UniqueHandle<UInt32> uniqueInt2 = uniqueInt.Detach();

			AssertEqual(uniqueInt.IsValid(), false, "Failed to detach handle.");
			AssertEqual(*uniqueInt2, 1, "Failed to reassign handle.");
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate detached handle.");

		return true;
	}

	bool UniqueHandleTests::Deallocate()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			UniqueHandle<UInt32> uniqueInt = MemoryManager::Allocate<UInt32>();

			AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes() - 4,
				"Failed to allocate handle.");

			uniqueInt.Deallocate();

			AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
				"Failed to deallocate handle.");
		}

		return true;
	}
}