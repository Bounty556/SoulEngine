/*
Tests for the TextFile class.
@file TextFileTests.cpp
@author Jacob Peterson
@edited 4/23/21
*/

#include "TextFileTests.h"

#include <IO/TextFile.h>
#include <TestsLib/TestMacros.h>
#include <UtilsLib/String.h>

namespace Soul
{
	void TextFileTests::RunAllTests()
	{
		RunTest(ReadExistingFile);
		RunTest(MoveTextFile);
		RunTest(WriteToNewFile);
	}

	bool TextFileTests::ReadExistingFile()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			TextFile textFile("C:/Programming/SoulEngine/SoulEngine/Data/Test.txt");
			String& textFileString = textFile.GetString();

			String expected = "This is a test!";

			AssertEqual(textFileString, expected, "Failed to read text file.");
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Incorrect deallocation of FileReader.");

		return true;
	}

	bool TextFileTests::MoveTextFile()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			TextFile textFile("C:/Programming/SoulEngine/SoulEngine/Data/Test.txt");
			TextFile textFile2(std::move(textFile));
			String& textFileString = textFile2.GetString();

			String expected = "This is a test!";

			AssertEqual(textFileString, expected, "Failed to read text file.");
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Incorrect deallocation of FileReader.");

		return true;
	}

	bool TextFileTests::WriteToNewFile()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			TextFile textFile("C:/Programming/SoulEngine/SoulEngine/Data/NewTest.txt");
			String& textFileString = textFile.GetString();

			textFileString = "Hey I'm writing new stuff to a file, cool!";

			textFile.WriteStringToFile();
		}

		{
			TextFile textFile("C:/Programming/SoulEngine/SoulEngine/Data/NewTest.txt");
			String& textFileString = textFile.GetString();

			String expected = "Hey I'm writing new stuff to a file, cool!";

			AssertEqual(textFileString, expected, "Failed to write new text file.");

			textFile.DeleteTextFile();
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Incorrect deallocation of FileReader.");

		return true;
	}
}