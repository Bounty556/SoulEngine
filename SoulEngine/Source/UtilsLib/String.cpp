/*
Used to manage C-Strings and provide utility functions and operations around
such strings.
@file String.cpp
@author Jacob Peterson
@edited 4/19/21
*/

#include "String.h"

#include <UtilsLib/Memory/MemoryManager.h>

namespace Soul
{
	String::String() :
		m_CString(MemoryManager::AllocateArray<char>(8)),
		m_Length(0),
		m_Capacity(8)
	{

	}

	String::String(const char* string) :
		m_CString(MemoryManager::AllocateArray<char>())
	{

	}

	String::String(String&& otherString)
	{

	}

	String::String(const String& otherString)
	{

	}

	String& String::operator=(const char* string)
	{

	}

	String& String::operator=(const String& otherString)
	{

	}

	String& String::operator=(String&& otherString)
	{

	}

	String& String::operator+(const char* string)
	{

	}

	String& String::operator+(const String& otherString)
	{

	}

	char String::operator[](Index index)
	{

	}

	String::operator const char* () const
	{

	}

	String String::Substring(Index start) const
	{

	}

	String String::Substring(Index start, Index end) const
	{

	}

	Index String::FindFirstOf(char findChar) const
	{

	}

	ArraySize String::Length() const
	{

	}

	const char* String::GetCString() const
	{

	}

	ArraySize String::StringLength(const char* string)
	{

	}
}