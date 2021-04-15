/*
Prints info to the console using three different logging levels.
@file Logger.h
@author Jacob Peterson
@edited 4/14/21
*/

#include "Logger.h"

#include <iostream>
#include <stdarg.h>

namespace Soul
{
	LogLevel Logger::m_LogLevel = LogLevel::Event;

	/*
	Logs an error to the console, with red text for distinguishability.
	*/
	void Logger::LogError(const char* message, ...)
	{
		if (m_LogLevel >= LogLevel::Error)
		{
			va_list arglist;
			HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

			/*
			Format string and print to console in red.
			*/
			SetConsoleTextAttribute(console, 4);
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
		if (m_LogLevel >= LogLevel::Warning)
		{
			va_list arglist;
			HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

			/*
			Format string and print to console in yellow.
			*/
			SetConsoleTextAttribute(console, 14);
			va_start(arglist, message);
			std::vprintf(message, arglist);
			va_end(arglist);
		}
	}

	void Logger::LogEvent(const char* message, ...)
	{
		if (m_LogLevel >= LogLevel::Event)
		{
			va_list arglist;
			HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

			/*
			Format string and print to console in green.
			*/
			SetConsoleTextAttribute(console, 10);
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
		if (m_LogLevel >= LogLevel::Info)
		{
			va_list arglist;
			HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

			/*
			Format string and print to console in white.
			*/
			SetConsoleTextAttribute(console, 15);
			va_start(arglist, message);
			std::vprintf(message, arglist);
			va_end(arglist);
		}
	}

	void Logger::SetLogLevel(LogLevel level)
	{
		m_LogLevel = level;
	}
}