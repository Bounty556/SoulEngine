/*
Used as a fake object for unit testing other classes.
@file TestClass.h
@author Jacob Peterson
@edited 4/14/21
*/

#pragma once

#include <UtilsLib/CommonTypes.h>

namespace Soul
{
	class TestClass
	{
	public:
		TestClass(UInt32 x, UInt8 y, Float32 z);
		TestClass(const TestClass& other);

		bool operator==(const TestClass& other) const;
		bool operator!=(const TestClass& other) const;

	private:
		UInt32 m_X;
		UInt8 m_Y;
		Float32 m_Z;
	};
}