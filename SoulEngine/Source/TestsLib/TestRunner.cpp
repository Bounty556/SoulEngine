/*
Used for aggregating and running tests. Assumes that all necessary engine
systems are currently running.
@file TestRunner.cpp
@author Jacob Peterson
@version 1 12/21/20
*/

#include "TestRunner.h"

namespace Soul
{
	void TestRunner::RunAllTestSuites()
	{
		/*
		TODO: Add test suites here.
		*/
	}

	void TestRunner::RunTestSuite(ITestSuite* coTestSuite)
	{
		coTestSuite->RunAllTests();
	}
}