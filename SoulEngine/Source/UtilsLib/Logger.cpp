/*
Prints info to the console using three different logging levels.
@file Logger.h
@author Jacob Peterson
@edited 12/17/20
*/

#include "Logger.h"

#include <iostream>
#include <stdarg.h>

namespace Soul
{
	LogLevel Logger::_seLogLevel = LogLevel::Info;

	/*
	Logs an error to the console, with red text for distinguishability.
	*/
	void Logger::LogError(const char* message, ...)
	{
		if (_seLogLevel >= LogLevel::Error)
		{
			va_list arglist;
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

			/*
			Format string and print to console in red.
			*/
			SetConsoleTextAttribute(hConsole, 4);
			va_start(arglist, message);
			std::vprintf(message, arglist);
			va_end(arglist);
		}
	}

	/*
	Logs an error to the console, with yellow text for distinguishability.
	*/
	void Logger::LogWarning(const char* message, ...)
	{
		if (_seLogLevel >= LogLevel::Warning)
		{
			va_list arglist;
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

			/*
			Format string and print to console in yellow.
			*/
			SetConsoleTextAttribute(hConsole, 14);
			va_start(arglist, message);
			std::vprintf(message, arglist);
			va_end(arglist);
		}
	}

	/*
	Logs an error to the console, with white text for distinguishability.
	*/
	void Logger::LogInfo(const char* message, ...)
	{
		if (_seLogLevel >= LogLevel::Info)
		{
			va_list arglist;
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

			/*
			Format string and print to console in white.
			*/
			SetConsoleTextAttribute(hConsole, 15);
			va_start(arglist, message);
			std::vprintf(message, arglist);
			va_end(arglist);
		}
	}

	void Logger::SetLogLevel(LogLevel eLevel)
	{
		_seLogLevel = eLevel;
	}
}