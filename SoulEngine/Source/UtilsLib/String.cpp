/*
Used to manage C-Strings and provide utility functions and operations around
such strings.
@file String.cpp
@author Jacob Peterson
@edited 4/19/21
*/

#include "String.h"

#include <cstring>

#include <UtilsLib/Memory/MemoryManager.h>
#include <UtilsLib/Macros.h>

namespace Soul
{
	String::String() :
		m_Length(0),
		m_Capacity(8),
		m_CString(MemoryManager::AllocateArray<char>(m_Capacity))
	{
	}

	String::String(const char* string) :
		m_Length(strlen(string)),
		m_Capacity(m_Length + 1),
		m_CString(MemoryManager::AllocateArray<char>(m_Capacity))
	{
		memcpy(m_CString.GetMemory(), string, m_Capacity);
	}

	String::String(String&& otherString) :
		m_Length(otherString.m_Length),
		m_Capacity(otherString.m_Capacity),
		m_CString(std::move(otherString.m_CString))
	{
		otherString.m_Length = 0;
		otherString.m_Capacity = 8;
	}

	String::String(const String& otherString) :
		m_Length(otherString.m_Length),
		m_Capacity(otherString.m_Capacity),
		m_CString(MemoryManager::AllocateArray<char>(m_Capacity))
	{
		memcpy(m_CString.GetMemory(), otherString.m_CString.GetMemory(), m_Capacity);
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