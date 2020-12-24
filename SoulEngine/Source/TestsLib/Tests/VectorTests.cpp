/*
Tests for the Vector class.
@file VectorTests.h
@author Jacob Peterson
@edited 12/23/2020
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
	}

	bool VectorTests::PrimitiveVector()
	{
		AssertEqual(MemoryManager::GetTotalAllocatedBytes(), 0,
			"Failed initial memory condition.");

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

		AssertEqual(MemoryManager::GetTotalAllocatedBytes(), 0,
			"Failed to deallocate primitive Vectors.");

		return true;
	}

	bool VectorTests::ObjectVector()
	{
		AssertEqual(MemoryManager::GetTotalAllocatedBytes(), 0,
			"Failed initial memory condition.");

		TestClass oFake(0, 'a', 1.5f);
		TestClass oFake2(1, 'b', 1.8f);

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

		AssertEqual(MemoryManager::GetTotalAllocatedBytes(), 0,
			"Failed to deallocate object Vectors.");

		return true;
	}

	bool VectorTests::VectorVector()
	{
		AssertEqual(MemoryManager::GetTotalAllocatedBytes(), 0,
			"Failed initial memory condition.");

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

		AssertEqual(MemoryManager::GetTotalAllocatedBytes(), 0,
			"Failed to deallocate Vectors of Vectors.");

		return true;
	}
}