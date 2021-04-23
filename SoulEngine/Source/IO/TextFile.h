/*
Used to create, open, and edit text files from the OS.
@file TextFile.h
@author Jacob Peterson
@edited 4/21/21
*/

#pragma once

#include <Memory/UniqueHandle.h>
#include <UtilsLib/String.h>

namespace Soul
{
	/*
	A file that uses a String as it's underlying buffer. Solely to be used
	for text files.

	The file is automatically read from when the object is created, and the
	underlying String object can be written and read from whenever desired.
	When you want to write back to the file, simply call WriteStringToFile(),
	and whatever is in the String will overwrite the contents of the file.
	*/
	class TextFile
	{
	public:
		TextFile(const char* fileName);
		TextFile(TextFile&& otherFile); 

		/*
		Writes the contents of the underlying String to the same file that was
		read.
		*/
		void WriteStringToFile();
		
		/*
		Deletes the file at the currently loaded file path.
		*/
		void DeleteTextFile();

		/*
		Gets the underlying String that acts as the buffer for this file reader.

		@return String reference containing file contents.
		*/
		String& GetString();

		TextFile() = delete;
		TextFile(const TextFile& otherFile) = delete;
		TextFile& operator=(TextFile&& otherFile) = delete;
		TextFile& operator=(const TextFile& otherFile) = delete;

	private:
		const char* m_FilePath; // Path of the file opened/created.
		String m_FileContents; // String containing file's contents.
	};
}