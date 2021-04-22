/*
Tests for the String class.
@file StringTests.h
@author Jacob Peterson
@edited 4/21/21
*/

#pragma once

#include <TestsLib/ITestSuite.h>

namespace Soul
{
	class StringTests : public ITestSuite
	{
	public:
		virtual void RunAllTests() override;

	private:
		bool InitString();
		bool AssignString();
		bool ConcatString();
		bool CompareStrings();
		bool CharCastString();
		bool Substring();
		bool IndexOfChar();
		bool EditChar();
	};
}