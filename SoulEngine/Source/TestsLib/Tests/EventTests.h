/*
Tests for the EventBus and EventListener classes.
@file EventTests.h
@author Jacob Peterson
@edited 12/26/2020
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
		bool SeveralEvents();
	};
}