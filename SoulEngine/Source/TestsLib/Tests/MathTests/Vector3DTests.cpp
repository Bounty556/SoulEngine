/*
Tests for the Vector3D class
@file Vector3DTests.cpp
@author Jacob Peterson
@edited 4/25/21
*/

#include "Vector3DTests.h"

#include <TestsLib/TestMacros.h>
#include <TestsLib/TestClass.h>
#include <UtilsLib/Maths/Vector3D.h>

namespace Soul
{
	void Vector3DTests::RunAllTests()
	{
		RunTest(VectorIndex);
		RunTest(MultiplyVectors);
		RunTest(DivideVectors);
		RunTest(SubtractVectors);
		RunTest(AddVectors);
		RunTest(VectorMagnitude);
		RunTest(NormalizeVector);
	}

	bool Vector3DTests::VectorIndex()
	{
		Vector3D vector(1.0f, 2.0f, 3.0f);

		AssertEqual(1.0f, vector[0], "Vector index failed.");
		AssertEqual(2.0f, vector.y, "Vector index failed.");
		AssertEqual(3.0f, vector[2], "Vector index failed.");

		return true;
	}

	bool Vector3DTests::MultiplyVectors()
	{
		Vector3D vector(1, 2, 3);
		vector *= 3;

		AssertEqual(vector.x, 3, "Failed to multiply vector.");

		Vector3D vector2 = vector * 2;

		AssertEqual(vector2.x, 6, "Failed to multiply vector.");

		return true;
	}

	bool Vector3DTests::DivideVectors()
	{
		Vector3D vector(4, 8, 16);
		vector /= 2;

		AssertEqual(vector.x, 2, "Failed to divide vector.");

		Vector3D vector2 = vector / 2;

		AssertEqual(vector2.x, 1, "Failed to divide vector.");

		return true;
	}

	bool Vector3DTests::SubtractVectors()
	{
		Vector3D vector(3, 4, 5);
		Vector3D vector2(1, 1, 1);
		vector -= vector2;

		AssertEqual(vector.x, 2, "Failed to subtract vectors.");

		Vector3D vector3 = -vector;

		AssertEqual(vector3.x, -2, "Failed to negate vector.");

		Vector3D vector4 = vector - vector2;

		AssertEqual(vector4.x, 1, "Failed to subtract vectors.");

		return true;
	}

	bool Vector3DTests::AddVectors()
	{
		Vector3D vector(3, 4, 5);
		Vector3D vector2(1, 1, 1);
		vector += vector2;

		AssertEqual(vector.x, 4, "Failed to add vector.");

		Vector3D vector3 = vector + vector2;

		AssertEqual(vector3.x, 5, "Failed to add vectors.");

		return true;
	}

	bool Vector3DTests::VectorMagnitude()
	{
		Vector3D vector(5, 0, 0);

		AssertClose(Magnitude(vector), 5, 0.01f, "Incorrect magnitude.");

		Vector3D vector2(10, -11, 12);

		AssertClose(Magnitude(vector2), 19.105f, 0.01f, "Incorrect magnitude.");

		return true;
	}

	bool Vector3DTests::NormalizeVector()
	{
		Vector3D vector(5, 0, 0);
		Vector3D unitVector = Normalize(vector);

		AssertEqual(Magnitude(unitVector), 1, "Failed to normalize.");
		AssertClose(unitVector.x, 1, 0.01f, "Incorrect unit values.");

		Vector3D vector2(1, 1, 1);
		Vector3D unitVector2 = Normalize(vector2);

		AssertEqual(Magnitude(unitVector2), 1, "Failed to normalize.");
		AssertClose(unitVector2.x, 0.5774f, 0.01f, "Incorrect unit values.");

		return true;
	}
}