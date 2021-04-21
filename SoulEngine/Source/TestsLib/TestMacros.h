/*
Macros used for running unit tests.
@file TestMacros.h
@author Jacob Peterson
@edited 4/20/21
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

#define AssertNotEqual(x, y, msg) if ((x) == (y)) \
{ \
	SoulLogError("Test failed on line " Stringify(__LINE__) ". " msg); \
}

#define AssertTrue(x, msg) if (!(x)) \
{ \
	SoulLogError("Test failed on line " Stringify(__LINE__) ". " msg); \
}

#define AssertFalse(x, msg) if ((x)) \
{ \
	SoulLogError("Test failed on line " Stringify(__LINE__) ". " msg); \
}