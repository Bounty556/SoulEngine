/*
Tests for the Queue class.
@file QueueTests.cpp
@author Jacob Peterson
@edited 12/22/2020
*/

#include "QueueTests.h"

#include <TestsLib/TestMacros.h>
#include <UtilsLib/Containers/Queue.h>

namespace Soul
{
	void QueueTests::RunAllTests()
	{
		RunTest(PrimitiveQueue);
		RunTest(ObjectQueue);
		RunTest(QueueQueue);
	}

	bool QueueTests::PrimitiveQueue()
	{
		AssertEqual(Soul::MemoryManager::GetTotalAllocatedBytes(), 0, 
			"Failed initial memory condition.");

		{
			Queue<UInt32> oQueue(100);

			for (UInt32 i = 0; i < 100; ++i)
			{
				oQueue.Push(i);
			}

			for (UInt32 i = 0; i < 100; ++i)
			{
				AssertEqual(oQueue.Pop(), i,
					"Queue did not store primitive correctly.");
			}

			Queue<UInt32> oQueue2(100);

			for (UInt32 i = 0; i < 100; ++i)
			{
				oQueue2.Push(i + 1);
			}

			oQueue = std::move(oQueue2);

			for (UInt32 i = 0; i < 100; ++i)
			{
				AssertEqual(oQueue.Pop(), i + 1, 
					"Primitive queue was not moved.");
			}
		}

		AssertEqual(Soul::MemoryManager::GetTotalAllocatedBytes(), 0,
			"Primitive queue was not properly deallocated.");

		return true;
	}

	bool QueueTests::ObjectQueue()
	{
		AssertEqual(Soul::MemoryManager::GetTotalAllocatedBytes(), 0,
			"Failed initial memory condition.");

		TestClass oFake(0, 'a', 2.0f);
		TestClass oFake2(1, 'b', 3.0f);

		{
			Queue<TestClass> oQueue(100);

			for (UInt32 i = 0; i < 100; ++i)
			{
				oQueue.Push(oFake);
			}

			for (UInt32 i = 0; i < 100; ++i)
			{
				AssertEqual(oQueue.Pop(), oFake,
					"Queue did not store object correctly.");
			}

			Queue<TestClass> oQueue2(100);

			for (UInt32 i = 0; i < 100; ++i)
			{
				oQueue2.Push(oFake2);
			}

			oQueue = std::move(oQueue2);

			for (UInt32 i = 0; i < 100; ++i)
			{
				AssertEqual(oQueue.Pop(), oFake2,
					"Object queue was not moved.");
			}
		}

		AssertEqual(Soul::MemoryManager::GetTotalAllocatedBytes(), 0,
			"Object queue was not properly deallocated.");

		return true;
	}

	bool QueueTests::QueueQueue()
	{
		AssertEqual(Soul::MemoryManager::GetTotalAllocatedBytes(), 0,
			"Failed initial memory condition.");

		{
			Queue<Queue<int>> oQueue(100);

			for (UInt32 i = 0; i < 100; ++i)
			{
				Queue<int> oTempQueue(50);

				for (UInt32 j = 0; j < 50; ++j)
				{
					oTempQueue.Push(j);
				}

				oQueue.Push(std::move(oTempQueue));
			}

			for (UInt32 i = 0; i < 100; ++i)
			{
				Queue<int> oTempQueue = oQueue.Pop();

				AssertEqual(oTempQueue.Pop(), 0,
					"Queue did not store queue correctly.");
			}

			Queue<Queue<int>> oQueue2(100);

			for (UInt32 i = 0; i < 100; ++i)
			{
				Queue<int> oTempQueue(50);

				for (UInt32 j = 0; j < 50; ++j)
				{
					oTempQueue.Push(j + 1);
				}

				oQueue2.Push(std::move(oTempQueue));
			}

			oQueue = std::move(oQueue2);

			for (UInt32 i = 0; i < 100; ++i)
			{
				Queue<int> oTempQueue = oQueue.Pop();

				AssertEqual(oTempQueue.Pop(), 1,
					"Queue queue was not moved.");
			}
		}

		AssertEqual(Soul::MemoryManager::GetTotalAllocatedBytes(), 0,
			"Queue queue was not properly deallocated.");

		return true;
	}
}