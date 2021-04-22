/*
Tests for the String class.
@file StringTests.cpp
@author Jacob Peterson
@edited 4/21/21
*/

#include "StringTests.h"

#include <TestsLib/TestMacros.h>
#include <UtilsLib/Logger.h>
#include <UtilsLib/String.h>

namespace Soul
{
	void StringTests::RunAllTests()
	{
		RunTest(InitString);
		RunTest(AssignString);
		RunTest(ConcatString);
		RunTest(CompareStrings);
		RunTest(CharCastString);
		RunTest(Substring);
		RunTest(IndexOfChar);
	}

	bool StringTests::InitString()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			String string("This is a C-String.");

			AssertEqual(string[10], 'C', "Failed to store C-String.");
			AssertEqual(string.Length(), 19, "Incorrect C-String Length.");
		}

		{
			String string("String 1");
			String string2(string);

			AssertEqual(string2[2], 'r', "Failed to copy string.");
			AssertEqual(string2.Length(), string.Length(), "Incorrect copy length.");
			AssertEqual(string2.Length(), 8, "Incorrect copy length.");
		}

		{
			String string("String 1");
			ByteCount stringBytes = MemoryManager::GetTotalAllocatedBytes();
			String string2(std::move(string));

			AssertEqual(stringBytes, MemoryManager::GetTotalAllocatedBytes(),
				"String not moved.");
			AssertEqual(string2[3], 'i', "Failed to move string.");
			AssertEqual(string2.Length(), 8, "Incorrect move length.");
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate Strings.");

		return true;
	}

	bool StringTests::AssignString()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			String string;
			string = "This is a C-String.";

			AssertEqual(string[10], 'C', "Failed to store C-String.");
			AssertEqual(string.Length(), 19, "Incorrect C-String Length.");
		}

		{
			String string("String 1");
			String string2;
			string2 = string;

			AssertEqual(string2[2], 'r', "Failed to copy string.");
			AssertEqual(string2.Length(), string.Length(), "Incorrect copy length.");
			AssertEqual(string2.Length(), 8, "Incorrect copy length.");
		}

		{
			String string("String 1");
			ByteCount stringBytes = MemoryManager::GetTotalAllocatedBytes();
			String string2;
			string2 = std::move(string);

			AssertEqual(stringBytes, MemoryManager::GetTotalAllocatedBytes(),
				"String not moved.");
			AssertEqual(string2[3], 'i', "Failed to move string.");
			AssertEqual(string2.Length(), 8, "Incorrect move length.");
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate Strings.");

		return true;
	}

	bool StringTests::ConcatString()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			String string1("This is ");
			const char* string2 = "a concatenated string.";
			String string3(string1 + string2);

			AssertEqual(string3[4], ' ', "Failed to concatenate string1.");
			AssertEqual(string3[15], 't', "Failed to concatenate string2.");
			AssertEqual(string3.Length(), 30, "Incorrect concatenated length.");
		}

		{
			String string1("This is ");
			String string2 = "a concatenated string.";
			String string3(string1 + string2);

			AssertEqual(string3[4], ' ', "Failed to concatenate string1.");
			AssertEqual(string3[15], 't', "Failed to concatenate string2.");
			AssertEqual(string3.Length(), 30, "Incorrect concatenated length.");
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate Strings.");

		return true;
	}

	bool StringTests::CompareStrings()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			String string1("String A");
			String string2("String A");
			String string3("String B");

			AssertTrue(string1 == string2, "String equality failed.");
			AssertTrue(string1 != string3, "String inequality failed.");
		}

		{
			String string1("String A");
			const char* string2 = "String A";
			const char* string3 = "String B";

			AssertTrue(string1 == string2, "String equality failed.");
			AssertTrue(string1 != string3, "String inequality failed.");
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate Strings.");

		return true;
	}

	bool StringTests::CharCastString()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			String string("Testing");

			AssertEqual(string.Length(), strlen(string), "String conversion failed.");
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate Strings.");

		return true;
	}

	bool StringTests::Substring()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			String string1("Testing");
			String string2(string1.Substring(2));
			String string3(string1.Substring(2, 3));

			AssertEqual(string2[1], 't', "Substring construction failed.");
			AssertEqual(string2.Length(), 5, "Substring construction failed.");
			AssertEqual(string3[0], 's', "Substring construction failed.");
			AssertEqual(string3.Length(), 1, "Substring construction failed.");
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate Strings.");

		return true;
	}

	bool StringTests::IndexOfChar()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			String string("testing");

			AssertEqual(string.FindFirstOf('t'), 0, "Failed to find 't'.");
			AssertEqual(string.FindFirstOf('i'), 4, "Failed to find 'i'.");
			AssertEqual(string.FindFirstOf('x'), -1, "Found incorrect character.");
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate Strings.");

		return true;
	}

	bool StringTests::EditChar()
	{
		ByteCount initialBytes = MemoryManager::GetTotalAllocatedBytes();

		{
			String string("Testing");

			string[0] = 'R';

			AssertEqual(string[0], 'R', "Failed to edit character in string.");
		}

		AssertEqual(initialBytes, MemoryManager::GetTotalAllocatedBytes(),
			"Failed to deallocate Strings.");

		return true;
	}
}