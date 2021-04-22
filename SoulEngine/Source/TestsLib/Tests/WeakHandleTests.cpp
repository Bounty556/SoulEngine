/*
Tests for the WeakHandle class.
@file WeakHandleTests.cpp
@author Jacob Peterson
@edited 4/21/21
*/

#include "WeakHandleTests.h"

#include <Memory/MemoryManager.h>
#include <Memory/UniqueHandle.h>
#include <Memory/WeakHandle.h>
#include <TestsLib/TestMacros.h>

namespace Soul
{
	void WeakHandleTests::RunAllTests()
	{
		RunTest(PrimitiveHandle);
		RunTest(ObjectHandle);
	}

	bool WeakHandleTests::PrimitiveHandle()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			UniqueHandle<int> uniqueInt = MemoryManager::Allocate<int>(0);
			{
				WeakHandle<int> weakInt(uniqueInt);
				AssertEqual(*weakInt, 0, "Failed to assign to WeakHandle.");
			}
			AssertEqual(*uniqueInt, 0, "Handle deleted.");
		}

		AssertEqual(MemoryManager::GetTotalAllocatedBytes(), initialBytes,
			"Failed to deallocate primitives.");

		return true;
	}

	bool WeakHandleTests::ObjectHandle()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		TestClass fakeClass = { 1, 'a', 3.24f };

		{
			UniqueHandle<TestClass> uniqueObj =
				MemoryManager::Allocate<TestClass>(fakeClass);
			{
				WeakHandle<TestClass> weakObj(uniqueObj);
				AssertEqual(*weakObj, fakeClass, "Failed to assign to WeakHandle.");
			}
			AssertEqual(*uniqueObj, fakeClass, "Handle deleted.");
		}

		AssertEqual(MemoryManager::GetTotalAllocatedBytes(), initialBytes,
			"Failed to deallocate object.");

		return true;
	}
}