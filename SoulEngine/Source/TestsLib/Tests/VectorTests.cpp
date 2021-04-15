/*
Tests for the Vector class.
@file VectorTests.h
@author Jacob Peterson
@edited 4/14/21
*/

#include "VectorTests.h"

#include <TestsLib/TestClass.h>
#include <TestsLib/TestMacros.h>
#include <UtilsLib/Containers/Vector.h>
#include <UtilsLib/Memory/MemoryManager.h>

namespace Soul
{
	void VectorTests::RunAllTests()
	{
		RunTest(PrimitiveVector);
		RunTest(ObjectVector);
		RunTest(VectorVector);
		RunTest(ResizeVector);
		RunTest(RemoveElements);
	}

	bool VectorTests::PrimitiveVector()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			Vector<UInt32> intVector(255);

			for (UInt8 i = 0; i < 255; ++i)
			{
				intVector.Push(i);
			}

			AssertEqual(intVector.Length(), 255, "Incorrect Vector length.");

			for (UInt8 i = 0; i < 255; ++i)
			{
				AssertEqual(intVector.Pop(), 254 - i,
					"Failed to store primitive in Vector.");
			}

			Vector<UInt32> intVector2(255);

			for (UInt8 i = 0; i < 255; ++i)
			{
				intVector2.Push(i + 1);
			}

			intVector = std::move(intVector2);

			AssertEqual(intVector.Length(), 255, "Incorrect Vector length.");

			for (UInt8 i = 0; i < 255; ++i)
			{
				AssertEqual(intVector.Pop(), 255 - i,
					"Failed to move primitive Vector.");
			}
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate primitive Vectors.");

		return true;
	}

	bool VectorTests::ObjectVector()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		TestClass fakeClass = { 0, 'a', 1.5f };
		TestClass fakeClass2 = { 1, 'b', 1.8f };

		{
			Vector<TestClass> classVector(255);

			for (UInt8 i = 0; i < 255; ++i)
			{
				classVector.Push(fakeClass);
			}

			AssertEqual(classVector.Length(), 255, "Incorrect Vector length.");

			for (UInt8 i = 0; i < 255; ++i)
			{
				AssertEqual(classVector.Pop(), fakeClass,
					"Failed to store object in Vector.");
			}

			Vector<TestClass> classVector2(255);

			for (UInt8 i = 0; i < 255; ++i)
			{
				classVector2.Push(fakeClass2);
			}

			classVector = std::move(classVector2);

			AssertEqual(classVector.Length(), 255, "Incorrect Vector length.");

			for (UInt8 i = 0; i < 255; ++i)
			{
				AssertEqual(classVector.Pop(), fakeClass2,
					"Failed to move object Vector.");
			}
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate object Vectors.");

		return true;
	}

	bool VectorTests::VectorVector()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			Vector<Vector<int>> intVectorVector(255);

			for (UInt8 i = 0; i < 255; ++i)
			{
				Vector<int> tempIntVector(255);

				for (UInt8 j = 0; j < 255; ++j)
				{
					tempIntVector.Push(j);
				}

				intVectorVector.Push(std::move(tempIntVector));
			}

			AssertEqual(intVectorVector.Length(), 255, "Incorrect Vector length.");

			Vector<int> tempIntVector(intVectorVector.Pop());

			AssertEqual(tempIntVector.Length(), 255, "Incorrect Vector length.");

			AssertEqual(tempIntVector.Pop(), 254,
				"Failed to store Vector in Vector.");

			Vector<Vector<int>> intVectorVector2(255);

			for (UInt8 i = 0; i < 255; ++i)
			{
				Vector<int> innerTempVector(255);

				for (UInt8 j = 0; j < 255; ++j)
				{
					innerTempVector.Push(j + 1);
				}

				intVectorVector2.Push(std::move(innerTempVector));
			}

			intVectorVector = std::move(intVectorVector2);

			tempIntVector = std::move(intVectorVector.Pop());

			AssertEqual(tempIntVector.Pop(), 255,
				"Failed to move Vector of Vectors.");
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate Vectors of Vectors.");

		return true;
	}

	bool VectorTests::ResizeVector()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			Vector<UInt8> smallVector(3);

			for (UInt8 i = 0; i < 255; ++i)
			{
				smallVector.Push(i);
			}
			AssertEqual(smallVector.Length(), 255, "Could not resize Vector");

			for (UInt8 i = 0; i < 255; ++i)
			{
				AssertEqual(smallVector.Pop(), 254 - i,
					"Incorrect values in resized Vector.");
			}

			Vector<TestClass> smallVector2(3);
			TestClass fakeClass = {1, 'a', 1.3f};

			for (UInt8 i = 0; i < 255; ++i)
			{
				smallVector2.Push(fakeClass);
			}
			for (UInt8 i = 0; i < 255; ++i)
			{
				AssertEqual(smallVector2.Pop(), fakeClass,
					"Failed to resize Vector of objects.");
			}

			Vector<Vector<int>> smallVector3(3);

			for (UInt8 i = 0; i < 255; ++i)
			{
				Vector<int> tempVector(3);

				for (UInt8 i = 0; i < 255; ++i)
				{
					tempVector.Push(i);
				}
				smallVector3.Push(std::move(tempVector));
			}
			for (UInt8 i = 0; i < 255; ++i)
			{
				Vector<int> tempVector(smallVector3.Pop());
				AssertEqual(tempVector.Pop(), 254,
					"Failed to resize Vector of Vectors");
			}
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate resized Vector.");

		return true;
	}

	bool VectorTests::RemoveElements()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			/*
			Test with primitives.
			*/
			Vector<int> smallVector(5);

			for (UInt8 i = 0; i < 5; ++i)
			{
				smallVector.Push(i);
			}

			smallVector.Remove(0);
			smallVector.Remove(3);

			AssertEqual(smallVector.Length(), 3,
				"Incorrect Vector length after removing elements.");

			AssertEqual(smallVector.Pop(), 3, "Incorrect element in Vector.");
			AssertEqual(smallVector.Pop(), 2, "Incorrect element in Vector.");
			AssertEqual(smallVector.Pop(), 1, "Incorrect element in Vector.");

			/*
			Test with basic objects.
			*/
			Vector<TestClass> smallVector2(5);

			TestClass fakeClass = { 1, 'a', 1.2f };

			for (UInt8 i = 0; i < 5; ++i)
			{
				smallVector2.Push(fakeClass);
			}

			smallVector2.Remove(0);
			smallVector2.Remove(3);

			AssertEqual(smallVector2.Pop(), fakeClass, "Incorrect object in Vector.");
			AssertEqual(smallVector2.Pop(), fakeClass, "Incorrect object in Vector.");
			AssertEqual(smallVector2.Pop(), fakeClass, "Incorrect object in Vector.");

			/*
			Test with other Vectors
			*/
			Vector<Vector<int>> smallVector3(5);

			for (UInt8 i = 0; i < 5; ++i)
			{
				Vector<int> tempVector(5);

				for (UInt8 i = 0; i < 5; ++i)
				{
					tempVector.Push(i);
				}

				tempVector.Remove(i);
				smallVector3.Push(std::move(tempVector));
			}

			smallVector3.Remove(0);
			smallVector3.Remove(3);

			Vector<int> oTemp(smallVector3.Pop());
			AssertEqual(oTemp.Pop(), 4, "Incorrect Vector removed.");
			AssertEqual(oTemp.Pop(), 2, "Incorrect Vector removed.");
			AssertEqual(oTemp.Pop(), 1, "Incorrect Vector removed.");
			AssertEqual(oTemp.Pop(), 0, "Incorrect Vector removed.");
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate Vectors with removed elements.");

		return true;
	}
}