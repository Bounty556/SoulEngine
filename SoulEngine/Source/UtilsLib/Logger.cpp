#include "Logger.h"

#include <iostream>
#include <stdarg.h>

namespace Soul
{
	LogLevel Logger::_seLogLevel = LogLevel::Info;

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