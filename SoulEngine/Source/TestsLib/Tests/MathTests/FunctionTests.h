/*
Tests for the functions in Functions.h
@file FunctionTests.h
@author Jacob Peterson
@edited 4/25/21
*/

#pragma once

#include <TestsLib/ITestSuite.h>

namespace Soul
{
	class FunctionTests : public ITestSuite
	{
	public:
		virtual void RunAllTests() override;

	private:
		bool SquareRootTest();
	};
}
