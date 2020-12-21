/*
Interface for writing test suites to be run with a TestRunner.
@file ITestSuite.h
@author Jacob Peterson
@version 1 12/21/20
*/

#pragma once

namespace Soul
{
	class ITestSuite
	{
	public:
		virtual void RunAllTests() = 0;
	};
}