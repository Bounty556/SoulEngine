/*
Tests for the Queue class.
@file QueueTests.cpp
@author Jacob Peterson
@edited 4/14/21
*/

#include "QueueTests.h"

#include <TestsLib/TestMacros.h>
#include <UtilsLib/CommonTypes.h>
#include <UtilsLib/Containers/Queue.h>
#include <UtilsLib/Memory/MemoryManager.h>

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
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();
		
		{
			Queue<UInt32> intQueue(100);

			for (UInt32 i = 0; i < 100; ++i)
			{
				intQueue.Push(i);
			}

			for (UInt32 i = 0; i < 100; ++i)
			{
				AssertEqual(intQueue.Pop(), i,
					"Queue did not store primitive correctly.");
			}

			Queue<UInt32> intQueue2(100);

			for (UInt32 i = 0; i < 100; ++i)
			{
				intQueue2.Push(i + 1);
			}

			intQueue = std::move(intQueue2);

			for (UInt32 i = 0; i < 100; ++i)
			{
				AssertEqual(intQueue.Pop(), i + 1, 
					"Primitive queue was not moved.");
			}
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Primitive queue was not properly deallocated.");

		return true;
	}

	bool QueueTests::ObjectQueue()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		TestClass fakeClass(0, 'a', 2.0f);
		TestClass fakeClass2(1, 'b', 3.0f);

		{
			Queue<TestClass> classQueue(100);

			for (UInt32 i = 0; i < 100; ++i)
			{
				classQueue.Push(fakeClass);
			}

			for (UInt32 i = 0; i < 100; ++i)
			{
				AssertEqual(classQueue.Pop(), fakeClass,
					"Queue did not store object correctly.");
			}

			Queue<TestClass> classQueue2(100);

			for (UInt32 i = 0; i < 100; ++i)
			{
				classQueue2.Push(fakeClass2);
			}

			classQueue = std::move(classQueue2);

			for (UInt32 i = 0; i < 100; ++i)
			{
				AssertEqual(classQueue.Pop(), fakeClass2,
					"Object queue was not moved.");
			}
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Object queue was not properly deallocated.");

		return true;
	}

	bool QueueTests::QueueQueue()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			Queue<Queue<int>> queueOfIntQueues(100);

			for (UInt32 i = 0; i < 100; ++i)
			{
				Queue<int> tempQueue(50);

				for (UInt32 j = 0; j < 50; ++j)
				{
					tempQueue.Push(j);
				}

				queueOfIntQueues.Push(std::move(tempQueue));
			}

			for (UInt32 i = 0; i < 100; ++i)
			{
				Queue<int> tempQueue = queueOfIntQueues.Pop();

				AssertEqual(tempQueue.Pop(), 0,
					"Queue did not store queue correctly.");
			}

			Queue<Queue<int>> queueOfIntQueues2(100);

			for (UInt32 i = 0; i < 100; ++i)
			{
				Queue<int> tempQueue(50);

				for (UInt32 j = 0; j < 50; ++j)
				{
					tempQueue.Push(j + 1);
				}

				queueOfIntQueues2.Push(std::move(tempQueue));
			}

			queueOfIntQueues = std::move(queueOfIntQueues2);

			for (UInt32 i = 0; i < 100; ++i)
			{
				Queue<int> tempQueue = queueOfIntQueues.Pop();

				AssertEqual(tempQueue.Pop(), 1,
					"Queue queue was not moved.");
			}
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Queue queue was not properly deallocated.");

		return true;
	}
}