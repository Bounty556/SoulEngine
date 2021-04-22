/*
Used to create, open, and edit text files from the OS.
@file TextFile.cpp
@author Jacob Peterson
@edited 4/21/21
*/

#include "TextFile.h"

#include <UtilsLib/Logger.h>
#include <Memory/MemoryManager.h>
#include <UtilsLib/Macros.h>

#include <Windows.h>

namespace Soul
{
	TextFile::TextFile(const char* fileName) :
		m_FilePath(fileName),
		m_FileContents()
	{
		HANDLE fileHandle = CreateFileA(m_FilePath, GENERIC_READ, 0, 0, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL);
		
		// TODO: Provide alternative?
		Assert(fileHandle != INVALID_HANDLE_VALUE);

		LARGE_INTEGER fileSize;
		// TODO: Provide alternative?
		Assert(GetFileSizeEx(fileHandle, &fileSize));

		char* fileText = MemoryManager::AllocateVolatile<char>(fileSize.QuadPart + 1);

		DWORD bytesRead;
		// TODO: Provide alternative?
		Assert(ReadFile(fileHandle, fileText, (DWORD)fileSize.QuadPart, &bytesRead, 0));

		// We need to add a null terminator since the file doesn't contain one
		fileText[fileSize.QuadPart] = '\0';

		m_FileContents = fileText;

		CloseHandle(fileHandle);
	}

	TextFile::TextFile(TextFile&& otherFile) :
		m_FilePath(otherFile.m_FilePath),
		m_FileContents(std::move(otherFile.m_FileContents))
	{
	}

	void TextFile::WriteStringToFile()
	{
		DeleteFileA(m_FilePath);
		HANDLE fileHandle = CreateFileA(m_FilePath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL);

		// TODO: Provide alternative?
		Assert(fileHandle != INVALID_HANDLE_VALUE);

		DWORD bytesWritten;
		// TODO: Provide alternative?
		Assert(WriteFile(fileHandle, m_FileContents.GetCString(), m_FileContents.Length(),
			&bytesWritten, 0));

		CloseHandle(fileHandle);
	}

	String& TextFile::GetString()
	{
		return m_FileContents;
	}
}