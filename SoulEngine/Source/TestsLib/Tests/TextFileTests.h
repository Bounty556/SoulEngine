/*
Tests for the TextFile class.
@file TextFileTests.h
@author Jacob Peterson
@edited 4/21/21
*/

#pragma once

#include <TestsLib/ITestSuite.h>

namespace Soul
{
	class TextFileTests : public ITestSuite
	{
	public:
		virtual void RunAllTests() override;

	private:
		bool ReadExistingFile();
		bool MoveTextFile();
		bool WriteToNewFile();
	};
}