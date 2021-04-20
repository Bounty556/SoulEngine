/*
Used to manage C-Strings and provide utility functions and operations around
such strings.
@file String.h
@author Jacob Peterson
@edited 4/19/21
*/

#pragma once

#include <UtilsLib/CommonTypes.h>
#include <UtilsLib/Memory/UniqueHandle.h>

namespace Soul
{
	class String
	{
	public:
		String();
		String(const char* string);
		String(String&& otherString);
		String(const String& otherString);

		String& operator=(const char* string);
		String& operator=(const String& otherString);
		String& operator=(String&& otherString);
		String& operator+(const char* string);
		String& operator+(const String& otherString);
		char operator[](Index index);
		operator const char*() const;

		/*
		Returns a String containing the characters from start index [inclusive]
		to the end of this string.

		@param start - Start index for the substring [inclusive].

		@return - String object containing the resulting substring.
		*/
		String Substring(Index start) const;

		/*
		Returns a String containing the characters from start index [inclusive]
		to the end index [exclusive].

		@param start - Start index for the substring [inclusive].

		@param end - End index for the substring [exclusive].

		@return - String object containing the resulting substring.
		*/
		String Substring(Index start, Index end) const;

		/*
		Returns the index of the first instance of the provided character in 
		this String.

		@param findChar - The character to find in this String.

		@return - Index of the first instance of the provided character.
		*/
		Index FindFirstOf(char findChar) const;

		/*
		Returns the length of this String.

		@return - Length of the string.
		*/
		ArraySize Length() const;

		/*
		Get's the underlying C-String for this String.

		@return - Pointer to the underlying C-String.
		*/
		const char* GetCString() const;

		/*
		Returns the Length of the provided C-String.
		*/
		static ArraySize StringLength(const char* string);

	private:
		ArraySize m_Length; // Length of currently stored C-String
		ArraySize m_Capacity; // Capacity of m_CString, including null terminator
		UniqueHandle<char> m_CString; // Handle to the C-String
	};
}