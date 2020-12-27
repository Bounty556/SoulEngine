/*
Tests for the Queue class.
@file QueueTests.h
@author Jacob Peterson
@edited 12/26/2020
*/

#pragma once

#include <TestsLib/ITestSuite.h>
#include <TestsLib/TestClass.h>

namespace Soul
{
	/*
	Tests for the Queue class.
	*/
	class QueueTests : public ITestSuite
	{
	protected:
		virtual void RunAllTests() override;

	private:
		bool PrimitiveQueue();
		bool ObjectQueue();
		bool QueueQueue();
	};
}