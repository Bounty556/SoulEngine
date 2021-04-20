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
		m_Length = strlen(string);
		if (m_Capacity < m_Length + 1)
		{
			m_Capacity = m_Length + 1;
			m_CString = MemoryManager::AllocateArray<char>(m_Capacity);
		}
		memcpy(m_CString.GetMemory(), string, m_Length + 1);

		return *this;
	}

	String& String::operator=(String&& otherString)
	{
		m_Length = otherString.m_Length;
		m_Capacity = otherString.m_Capacity;
		m_CString = std::move(otherString.m_CString);

		return *this;
	}
	
	String& String::operator=(const String& otherString)
	{
		m_Length = otherString.m_Length;
		if (m_Capacity < m_Length + 1)
		{
			m_Capacity = m_Length + 1;
			m_CString = MemoryManager::AllocateArray<char>(m_Capacity);
		}
		memcpy(m_CString.GetMemory(), otherString.m_CString.GetMemory(), m_Length + 1);

		return *this;
	}

	String String::operator+(const char* string)
	{
		String tempString;
		tempString.m_Length = this->m_Length + strlen(string);
		tempString.m_Capacity = m_Length + 1;
		tempString.m_CString = MemoryManager::AllocateArray<char>(m_Capacity);
		memcpy(tempString.m_CString.GetMemory(), m_CString.GetMemory(), m_Length);
		memcpy(tempString.m_CString.GetMemory() + m_Length, string, strlen(string) + 1);

		return std::move(tempString);
	}

	String String::operator+(const String& otherString)
	{
		String tempString;
		tempString.m_Length = this->m_Length + otherString.Length();
		tempString.m_Capacity = m_Length + 1;
		tempString.m_CString = MemoryManager::AllocateArray<char>(m_Capacity);
		memcpy(tempString.m_CString.GetMemory(), m_CString.GetMemory(), m_Length);
		memcpy(tempString.m_CString.GetMemory() + m_Length,
			otherString.m_CString.GetMemory(), otherString.Length() + 1);

		return std::move(tempString);
	}

	bool String::operator==(const String& otherString)
	{
		if (otherString.Length() != m_Length)
		{
			return false;
		}

		for (Index i = 0; i < m_Length; i++)
		{
			if (otherString[i] != m_CString[i])
			{
				return false;
			}
		}

		return true;
	}

	bool String::operator==(const char* string)
	{
		if (strlen(string) != m_Length)
		{
			return false;
		}

		for (Index i = 0; i < m_Length; i++)
		{
			if (string[i] != m_CString[i])
			{
				return false;
			}
		}

		return true;
	}

	bool String::operator!=(const String& otherString)
	{
		return !operator==(otherString);
	}

	bool String::operator!=(const char* string)
	{
		return !operator==(string);
	}

	char& String::operator[](Index index)
	{
		return m_CString[index];
	}

	const char& String::operator[](Index index) const
	{
		return m_CString[index];
	}

	String::operator const char* () const
	{
		return m_CString.GetMemory();
	}

	String String::Substring(Index start) const
	{
		Assert(start >= 0 && start < m_Length);

		String tempString;
		tempString.m_Length = m_Length - start;
		tempString.m_Capacity = m_Length + 1;
		tempString.m_CString = MemoryManager::AllocateArray<char>(m_Capacity);
		memcpy(tempString.m_CString.GetMemory(), m_CString.GetMemory() + start,
			tempString.m_Capacity);

		return std::move(tempString);
	}

	String String::Substring(Index start, Index end) const
	{
		Assert(start >= 0 && start < m_Length && end > start && end <= m_Length);

		String tempString;
		tempString.m_Length = start - end;
		tempString.m_Capacity = m_Length + 1;
		tempString.m_CString = MemoryManager::AllocateArray<char>(m_Capacity);
		memcpy(tempString.m_CString.GetMemory(), m_CString.GetMemory() + start,
			tempString.m_Length);

		tempString[tempString.m_Length] = '\0';

		return std::move(tempString);
	}

	Int32 String::FindFirstOf(char findChar) const
	{
		for (Index i = 0; i < m_Length; i++)
		{
			if (m_CString[i] == findChar)
			{
				return (Int32)i;
			}
		}

		return -1;
	}

	ArraySize String::Length() const
	{
		return m_Length;
	}

	const char* String::GetCString() const
	{
		return m_CString.GetMemory();
	}
}