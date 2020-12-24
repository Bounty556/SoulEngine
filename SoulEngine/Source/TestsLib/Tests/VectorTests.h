/*
Tests for the Vector class.
@file VectorTests.h
@author Jacob Peterson
@edited 12/23/2020
*/

#pragma once

#include <TestsLib/ITestSuite.h>

namespace Soul
{
	/*
	Tests for the Vector class.
	*/
	class VectorTests : public ITestSuite
	{
	protected:
		virtual void RunAllTests() override;

	private:
		bool PrimitiveVector();
		bool ObjectVector();
		bool VectorVector();
	};
}