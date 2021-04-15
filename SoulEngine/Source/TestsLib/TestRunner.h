/*
Used for aggregating and running tests. Assumes that all necessary engine
systems are currently running.
@file TestRunner.h
@author Jacob Peterson
@edited 4/14/21
*/

#pragma once

#include <TestsLib/ITestSuite.h>

namespace Soul
{
	/*
	Used to run implemented test suites. To run individual tests, use
	RunTestSuite(...), passing in a new copy of a test suite. To run all
	implemented test suites, call RunAllTestSuites(). All tests report their
	results to the console.
	*/
	class TestRunner
	{
	public:
		TestRunner() = delete;
		
		/*
		Runs all implemented test suites.
		*/
		static void RunAllTestSuites();

		/*
		Runs a single provided test suite.

		@param testSuite - The test suite to be run.
		*/
		static void RunTestSuite(ITestSuite* testSuite);
	};
}