/*
Tests for the EventBus and EventListener classes.
@file EventTests.h
@author Jacob Peterson
@edited 4/15/21
*/

#pragma once

#include <TestsLib/ITestSuite.h>

namespace Soul
{
	class EventTests : public ITestSuite
	{
	public:
		virtual void RunAllTests() override;

	private:
		bool PrimitiveDataEvent();
		bool ObjectDataEvent();
		bool ExpandedCallbackVector();
		bool SeveralEvents();
	};
}