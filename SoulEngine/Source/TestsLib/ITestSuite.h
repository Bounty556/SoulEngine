/*
Interface for writing test suites to be run with a TestRunner.
@file ITestSuite.h
@author Jacob Peterson
@edited 12/21/20
*/

#pragma once

namespace Soul
{
	class TestRunner;

	/*
	Interface for writing test suites to be run with a TestRunner.
	*/
	class ITestSuite
	{
	protected:
		friend TestRunner;

	protected:
		/*
		Runs all tests in this test suite.
		*/
		virtual void RunAllTests() = 0;
	};
}