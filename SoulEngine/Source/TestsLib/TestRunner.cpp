/*
Used for aggregating and running tests. Assumes that all necessary engine
systems are currently running.
@file TestRunner.cpp
@author Jacob Peterson
@edited 4/25/21
*/

#include "TestRunner.h"

#include <TestsLib/TestMacros.h>
#include <TestsLib/Tests/EventTests.h>
#include <TestsLib/Tests/MathTests/FunctionTests.h>
#include <TestsLib/Tests/MathTests/Vector3DTests.h>
#include <TestsLib/Tests/MemoryManagerTests.h>
#include <TestsLib/Tests/UniqueHandleTests.h>
#include <TestsLib/Tests/QueueTests.h>
#include <TestsLib/Tests/StringTests.h>
#include <TestsLib/Tests/TextFileTests.h>
#include <TestsLib/Tests/VectorTests.h>
#include <TestsLib/Tests/WeakHandleTests.h>

namespace Soul
{
	void TestRunner::RunAllTestSuites()
	{
		/*
		TODO: Add test suites here.
		*/
		CreateTestSuite(MemoryManagerTests);
		CreateTestSuite(UniqueHandleTests);
		CreateTestSuite(QueueTests);
		CreateTestSuite(VectorTests);
		CreateTestSuite(EventTests);
		CreateTestSuite(WeakHandleTests);
		CreateTestSuite(StringTests);
		CreateTestSuite(TextFileTests);
		CreateTestSuite(FunctionTests);
		CreateTestSuite(Vector3DTests);
	}

	void TestRunner::RunTestSuite(ITestSuite* testSuite)
	{
		testSuite->RunAllTests();
	}
}