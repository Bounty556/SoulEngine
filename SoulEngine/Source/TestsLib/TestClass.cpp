/*
Used as a fake object for unit testing other classes.
@file TestClass.cpp
@author Jacob Peterson
@edited 4/14/21
*/

#include "TestClass.h"

namespace Soul
{
	TestClass::TestClass(UInt32 x, UInt8 y, Float32 z) :
		m_X(x),
		m_Y(y),
		m_Z(z)
	{

	}

	TestClass::TestClass(const TestClass& other) :
		m_X(other.m_X),
		m_Y(other.m_Y),
		m_Z(other.m_Z)
	{
	
	}

	bool TestClass::operator==(const TestClass& other) const
	{
		return other.m_X == m_X && other.m_Y == m_Y && other.m_Z == m_Z;
	}

	bool TestClass::operator!=(const TestClass& other) const
	{
		return other.m_X != m_X || other.m_Y != m_Y || other.m_Z != m_Z;
	}
}