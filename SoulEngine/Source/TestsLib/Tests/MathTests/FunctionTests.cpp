/*
Tests for the functions in Functions.h
@file FunctionTests.cpp
@author Jacob Peterson
@edited 4/25/21
*/

#include "FunctionTests.h"

#include <TestsLib/TestMacros.h>
#include <TestsLib/TestClass.h>
#include <UtilsLib/Maths/Functions.h>

namespace Soul
{
	void FunctionTests::RunAllTests()
	{
		RunTest(SquareRootTest);
	}

	bool FunctionTests::SquareRootTest()
	{
		Float32 x = SquareRoot(100.0f);
		AssertClose(10.0f, SquareRoot(100.0f), 0.01f, "SquareRoot failed.");
		AssertClose(17.25f, SquareRoot(297.57f), 0.01f, "SquareRoot failed.");

		return true;
	}
}