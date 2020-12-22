/*
Tests for the UniqueHandle class.
@file UniqueHandleTests.h
@author Jacob Peterson
@edited 12/22/2020
*/

#pragma once

#include <TestsLib/ITestSuite.h>
#include <TestsLib/TestClass.h>

namespace Soul
{
	/*
	Tests for the UniqueHandle class.
	*/
	class UniqueHandleTests : public ITestSuite
	{
	protected:
		virtual void RunAllTests() override;

	private:
		bool PrimitiveHandle();
		bool ObjectHandle();
		bool HandleHandle();
		bool PrimitiveArrayHandle();
		bool ObjectArrayHandle();
		bool HandleArrayHandle();
	};
}