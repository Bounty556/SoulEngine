/*
Tests for the WeakHandle class.
@file WeakHandleTests.h
@author Jacob Peterson
@edited 12/26/2020
*/

#pragma once

#include <TestsLib/ITestSuite.h>
#include <TestsLib/TestClass.h>

namespace Soul
{
	/*
	Tests for the WeakHandle class.
	*/
	class WeakHandleTests : public ITestSuite
	{
	protected:
		virtual void RunAllTests() override;

	private:
		bool PrimitiveHandle();
		bool ObjectHandle();
	};
}