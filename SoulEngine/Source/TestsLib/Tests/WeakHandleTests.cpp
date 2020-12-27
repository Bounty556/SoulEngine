/*
Tests for the WeakHandle class.
@file WeakHandleTests.cpp
@author Jacob Peterson
@edited 12/26/2020
*/

#include "WeakHandleTests.h"

#include <TestsLib/TestMacros.h>
#include <UtilsLib/Memory/MemoryManager.h>
#include <UtilsLib/Memory/UniqueHandle.h>
#include <UtilsLib/Memory/WeakHandle.h>

namespace Soul
{
	void WeakHandleTests::RunAllTests()
	{
		RunTest(PrimitiveHandle);
		RunTest(ObjectHandle);
	}

	bool WeakHandleTests::PrimitiveHandle()
	{
		ByteCount uiInitialBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			UniqueHandle<int> hUniqueInt = MemoryManager::Allocate<int>(0);
			{
				WeakHandle<int> hInt(hUniqueInt);
				AssertEqual(*hInt, 0, "Failed to assign to WeakHandle.");
			}
			AssertEqual(*hUniqueInt, 0, "Handle deleted.");
		}

		AssertEqual(MemoryManager::GetTotalAllocatedBytes(), uiInitialBytes,
			"Failed to deallocate primitives.");

		return true;
	}

	bool WeakHandleTests::ObjectHandle()
	{
		ByteCount uiInitialBytes = MemoryManager::GetTotalAllocatedBytes();

		TestClass oFake = { 1, 'a', 3.24f };

		{
			UniqueHandle<TestClass> hUniqueObj =
				MemoryManager::Allocate<TestClass>(oFake);
			{
				WeakHandle<TestClass> hObj(hUniqueObj);
				AssertEqual(*hObj, oFake, "Failed to assign to WeakHandle.");
			}
			AssertEqual(*hUniqueObj, oFake, "Handle deleted.");
		}

		AssertEqual(MemoryManager::GetTotalAllocatedBytes(), uiInitialBytes,
			"Failed to deallocate object.");

		return true;
	}
}