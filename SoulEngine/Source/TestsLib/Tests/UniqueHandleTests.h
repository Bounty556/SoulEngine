/*
Tests for the UniqueHandle class.
@file UniqueHandleTests.h
@author Jacob Peterson
@edited 12/21/2020
*/

#pragma once

#include <TestsLib/ITestSuite.h>

namespace Soul
{
	/*
	Tests for the UniqueHandle class.
	*/
	class UniqueHandleTests : public ITestSuite
	{
	private:
		class TestClass
		{
		public:
			TestClass(int iX, char iY, float fZ) :
				_iX(iX),
				_iY(iY),
				_fZ(fZ)
			{}
			
			TestClass(const TestClass& oOther) :
				_iX(oOther._iX),
				_iY(oOther._iY),
				_fZ(oOther._fZ)
			{}

			bool operator!=(const TestClass& oOther) const
			{
				if (oOther._iX != _iX ||
					oOther._iY != _iY ||
					oOther._fZ != _fZ)
				{
					return true;
				}

				return false;
			}

		private:
			int _iX;
			char _iY;
			float _fZ;
		};

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