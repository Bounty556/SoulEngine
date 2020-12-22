/*
Used as a fake object for unit testing other classes.
@file TestClass.h
@author Jacob Peterson
@edited 12/22/20
*/

#pragma once

#include <UtilsLib/CommonTypes.h>

namespace Soul
{
	class TestClass
	{
	public:
		TestClass(UInt32 iX, UInt8 iY, Float32 fZ);
		TestClass(const TestClass& oOther);

		bool operator!=(const TestClass& oOther) const;

	private:
		UInt32 _iX;
		UInt8 _iY;
		Float32 _fZ;
	};
}