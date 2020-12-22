/*
Used as a fake object for unit testing other classes.
@file TestClass.cpp
@author Jacob Peterson
@edited 12/22/20
*/

#include "TestClass.h"

namespace Soul
{
	TestClass::TestClass(UInt32 iX, UInt8 iY, Float32 fZ) :
		_iX(iX),
		_iY(iY),
		_fZ(fZ)
	{

	}

	TestClass::TestClass(const TestClass& oOther) :
		_iX(oOther._iX),
		_iY(oOther._iY),
		_fZ(oOther._fZ)
	{
	
	}

	bool TestClass::operator!=(const TestClass& oOther) const
	{
		if (oOther._iX != _iX ||
			oOther._iY != _iY ||
			oOther._fZ != _fZ)
		{
			return true;
		}

		return false;
	}
}