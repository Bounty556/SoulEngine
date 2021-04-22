/*
Tests for the TextFile class.
@file TextFileTests.cpp
@author Jacob Peterson
@edited 4/21/21
*/

#include "TextFileTests.h"

#include <IO/TextFile.h>
#include <TestsLib/TestMacros.h>

namespace Soul
{
	void TextFileTests::RunAllTests()
	{
		RunTest(ReadExistingFile);
		RunTest(MoveTextFile);
		RunTest(WriteToNewFile);
	}

	bool TextFileTests::ReadExistingFile()
	{
		return true;
	}

	bool TextFileTests::MoveTextFile()
	{
		return true;
	}

	bool TextFileTests::WriteToNewFile()
	{
		return true;
	}
}