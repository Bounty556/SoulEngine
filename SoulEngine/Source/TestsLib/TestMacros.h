/*
Macros used for running unit tests.
@file TestMacros.h
@author Jacob Peterson
@version 1 12/21/2020
*/

#pragma once

#include <UtilsLib/Macros.h>

#define RunTest(x) SoulLogInfo("Running test " Stringify(x)); \
if (x()) \
	SoulLogInfo("All tests passed!"); \
else \
	SoulLogError("Test failed!");

#define AssertEqual(x, y, msg) if ((x) != (y)) \
{ \
	SoulLogError("Test failed on line " Stringify(__LINE__) " in " __FILE__ ". " msg); \
	return false; \
}