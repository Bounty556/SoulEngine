/*
Macros used for running unit tests.
@file TestMacros.h
@author Jacob Peterson
@edited 12/21/2020
*/

#pragma once

#include <UtilsLib/Macros.h>
#include <UtilsLib/Logger.h>

#define CreateTestSuite(x) SoulLogWarning("Running test suite " Stringify(x)); \
RunTestSuite(new x);

#define RunTest(x) SoulLogInfo("Running test " Stringify(x)); \
if (x()) \
	SoulLogInfo("All tests passed!");

#define AssertEqual(x, y, msg) if ((x) != (y)) \
{ \
	SoulLogError("Test failed on line " Stringify(__LINE__) ". " msg); \
	return false; \
}