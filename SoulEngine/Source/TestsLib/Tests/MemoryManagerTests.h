/*
Tests for the MemoryManager class.
@file MemoryManagerTests.h
@author Jacob Peterson
@edited 12/26/2020
*/

#pragma once

#include <TestsLib/ITestSuite.h>

namespace Soul
{
	/*
	Tests for the MemoryManager class.
	*/
	class MemoryManagerTests : public ITestSuite
	{
	protected:
		virtual void RunAllTests() override;

	private:
		bool BasicAllocation();
		bool ArrayAllocation();
		bool VolatileAllocation();
	};
}