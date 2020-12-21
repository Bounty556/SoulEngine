/*
Used for aggregating and running tests. Assumes that all necessary engine
systems are currently running.
@file TestRunner.cpp
@author Jacob Peterson
@edited 12/21/20
*/

#include "TestRunner.h"

#include <TestsLib/TestMacros.h>
#include <TestsLib/Tests/MemoryManagerTests.h>
#include <TestsLib/Tests/UniqueHandleTests.h>

namespace Soul
{
	void TestRunner::RunAllTestSuites()
	{
		/*
		TODO: Add test suites here.
		*/
		CreateTestSuite(MemoryManagerTests);
		CreateTestSuite(UniqueHandleTests);
	}

	void TestRunner::RunTestSuite(ITestSuite* coTestSuite)
	{
		coTestSuite->RunAllTests();
	}
}