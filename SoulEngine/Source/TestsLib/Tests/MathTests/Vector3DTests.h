/*
Tests for the Vector3D class
@file Vector3DTests.h
@author Jacob Peterson
@edited 4/25/21
*/

#pragma once

#include <TestsLib/ITestSuite.h>

namespace Soul
{
	class Vector3DTests : public ITestSuite
	{
	public:
		virtual void RunAllTests() override;

	private:
		bool VectorIndex();
		bool MultiplyVectors();
		bool DivideVectors();
		bool SubtractVectors();
		bool AddVectors();
		bool VectorMagnitude();
		bool NormalizeVector();
	};
}
