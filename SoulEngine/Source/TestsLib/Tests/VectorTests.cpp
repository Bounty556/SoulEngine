/*
Tests for the Vector class.
@file VectorTests.h
@author Jacob Peterson
@edited 12/26/2020
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
		ByteCount uiBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			Vector<UInt32> oIntVector(255);

			for (UInt8 i = 0; i < 255; ++i)
			{
				oIntVector.Push(i);
			}

			AssertEqual(oIntVector.Length(), 255, "Incorrect Vector length.");

			for (UInt8 i = 0; i < 255; ++i)
			{
				AssertEqual(oIntVector.Pop(), 254 - i,
					"Failed to store primitive in Vector.");
			}

			Vector<UInt32> oIntVector2(255);

			for (UInt8 i = 0; i < 255; ++i)
			{
				oIntVector2.Push(i + 1);
			}

			oIntVector = std::move(oIntVector2);

			AssertEqual(oIntVector.Length(), 255, "Incorrect Vector length.");

			for (UInt8 i = 0; i < 255; ++i)
			{
				AssertEqual(oIntVector.Pop(), 255 - i,
					"Failed to move primitive Vector.");
			}
		}

		AssertEqual(uiBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate primitive Vectors.");

		return true;
	}

	bool VectorTests::ObjectVector()
	{
		ByteCount uiBytes = MemoryManager::GetTotalAllocatedBytes();

		TestClass oFake = { 0, 'a', 1.5f };
		TestClass oFake2 = { 1, 'b', 1.8f };

		{
			Vector<TestClass> oVector(255);

			for (UInt8 i = 0; i < 255; ++i)
			{
				oVector.Push(oFake);
			}

			AssertEqual(oVector.Length(), 255, "Incorrect Vector length.");

			for (UInt8 i = 0; i < 255; ++i)
			{
				AssertEqual(oVector.Pop(), oFake,
					"Failed to store object in Vector.");
			}

			Vector<TestClass> oVector2(255);

			for (UInt8 i = 0; i < 255; ++i)
			{
				oVector2.Push(oFake2);
			}

			oVector = std::move(oVector2);

			AssertEqual(oVector.Length(), 255, "Incorrect Vector length.");

			for (UInt8 i = 0; i < 255; ++i)
			{
				AssertEqual(oVector.Pop(), oFake2,
					"Failed to move object Vector.");
			}
		}

		AssertEqual(uiBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate object Vectors.");

		return true;
	}

	bool VectorTests::VectorVector()
	{
		ByteCount uiBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			Vector<Vector<int>> oVector(255);

			for (UInt8 i = 0; i < 255; ++i)
			{
				Vector<int> oTemp(255);

				for (UInt8 j = 0; j < 255; ++j)
				{
					oTemp.Push(j);
				}

				oVector.Push(std::move(oTemp));
			}

			AssertEqual(oVector.Length(), 255, "Incorrect Vector length.");

			Vector<int> oTemp(oVector.Pop());

			AssertEqual(oTemp.Length(), 255, "Incorrect Vector length.");

			AssertEqual(oTemp.Pop(), 254,
				"Failed to store Vector in Vector.");

			Vector<Vector<int>> oVector2(255);

			for (UInt8 i = 0; i < 255; ++i)
			{
				Vector<int> oTemp(255);

				for (UInt8 j = 0; j < 255; ++j)
				{
					oTemp.Push(j + 1);
				}

				oVector2.Push(std::move(oTemp));
			}

			oVector = std::move(oVector2);

			oTemp = std::move(oVector.Pop());

			AssertEqual(oTemp.Pop(), 255,
				"Failed to move Vector of Vectors.");
		}

		AssertEqual(uiBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate Vectors of Vectors.");

		return true;
	}

	bool VectorTests::ResizeVector()
	{
		ByteCount uiBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			Vector<UInt8> oSmallVector(3);

			for (UInt8 i = 0; i < 255; ++i)
			{
				oSmallVector.Push(i);
			}
			AssertEqual(oSmallVector.Length(), 255, "Could not resize Vector");

			for (UInt8 i = 0; i < 255; ++i)
			{
				AssertEqual(oSmallVector.Pop(), 254 - i,
					"Incorrect values in resized Vector.");
			}

			Vector<TestClass> oSmallVector2(3);
			TestClass oFake = {1, 'a', 1.3f};

			for (UInt8 i = 0; i < 255; ++i)
			{
				oSmallVector2.Push(oFake);
			}
			for (UInt8 i = 0; i < 255; ++i)
			{
				AssertEqual(oSmallVector2.Pop(), oFake,
					"Failed to resize Vector of objects.");
			}

			Vector<Vector<int>> oSmallVector3(3);

			for (UInt8 i = 0; i < 255; ++i)
			{
				Vector<int> oTemp(3);

				for (UInt8 i = 0; i < 255; ++i)
				{
					oTemp.Push(i);
				}
				oSmallVector3.Push(std::move(oTemp));
			}
			for (UInt8 i = 0; i < 255; ++i)
			{
				Vector<int> oTemp(oSmallVector3.Pop());
				AssertEqual(oTemp.Pop(), 254,
					"Failed to resize Vector of Vectors");
			}
		}

		AssertEqual(uiBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate resized Vector.");

		return true;
	}

	bool VectorTests::RemoveElements()
	{
		ByteCount uiBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			/*
			Test with primitives.
			*/
			Vector<int> oSmallVector(5);

			for (UInt8 i = 0; i < 5; ++i)
			{
				oSmallVector.Push(i);
			}

			oSmallVector.Remove(0);
			oSmallVector.Remove(3);

			AssertEqual(oSmallVector.Length(), 3,
				"Incorrect Vector length after removing elements.");

			AssertEqual(oSmallVector.Pop(), 3, "Incorrect element in Vector.");
			AssertEqual(oSmallVector.Pop(), 2, "Incorrect element in Vector.");
			AssertEqual(oSmallVector.Pop(), 1, "Incorrect element in Vector.");

			/*
			Test with basic objects.
			*/
			Vector<TestClass> oSmallVector2(5);

			TestClass oFake = { 1, 'a', 1.2f };

			for (UInt8 i = 0; i < 5; ++i)
			{
				oSmallVector2.Push(oFake);
			}

			oSmallVector2.Remove(0);
			oSmallVector2.Remove(3);

			AssertEqual(oSmallVector2.Pop(), oFake, "Incorrect object in Vector.");
			AssertEqual(oSmallVector2.Pop(), oFake, "Incorrect object in Vector.");
			AssertEqual(oSmallVector2.Pop(), oFake, "Incorrect object in Vector.");

			/*
			Test with other Vectors
			*/
			Vector<Vector<int>> oSmallVector3(5);

			for (UInt8 i = 0; i < 5; ++i)
			{
				Vector<int> oTemp(5);

				for (UInt8 i = 0; i < 5; ++i)
				{
					oTemp.Push(i);
				}

				oTemp.Remove(i);
				oSmallVector3.Push(std::move(oTemp));
			}

			oSmallVector3.Remove(0);
			oSmallVector3.Remove(3);

			Vector<int> oTemp(oSmallVector3.Pop());
			AssertEqual(oTemp.Pop(), 4, "Incorrect Vector removed.");
			AssertEqual(oTemp.Pop(), 2, "Incorrect Vector removed.");
			AssertEqual(oTemp.Pop(), 1, "Incorrect Vector removed.");
			AssertEqual(oTemp.Pop(), 0, "Incorrect Vector removed.");
		}

		AssertEqual(uiBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate Vectors with removed elements.");

		return true;
	}
}