/*
Prints info to the console using three different logging levels.
@file Logger.h
@author Jacob Peterson
@edited 12/17/20
*/

#pragma once

#include <Windows.h>

namespace Soul
{
	/*
	Determines which messages the logger prints and which
	messages it ignores.
	*/
	enum LogLevel
	{
		Error,
		Warning,
		Info
	};

	/*
	The Logger is used to print color-coded debug information out to
	the console. There are three log levels: Error, Warning, and
	Info. Setting a lighter log level (Warning, Info) will show the
	heavier log levels (Error, Warning).

	By default the log level is set to Info, so all logs are shown.
	You can change the log level with SetLogLevel().

	The LogError(), LogWarning(), and LogInfo() functions should
	not be used. Rather, the SoulLogError, SoulLogWarning and
	SoulLogInfo macros should be used to log.
	*/
	class Logger
	{
	public:

		// Member Functions ////////////////////////////////////////////////////

		/*
		Logs an error to the console, with red text for
		distinguishability.

		@param zMessage - Formatted C-String that gets printed to the console.

		@param ... - Parameters to pass to the formatted string to be printed.
		*/
		static void LogError(const char* zMessage, ...);

		/*
		Logs a warning to the console, with yellow text for
		distinguishability.

		@param zMessage - Formatted C-String that gets printed to the console.

		@param ... - Parameters to pass to the formatted string to be printed.
		*/
		static void LogWarning(const char* zMessage, ...);

		/*
		Logs info to the console, with white text for
		distinguishability.

		@param zMessage - Formatted C-String that gets printed to the console.

		@param ... - Parameters to pass to the formatted string to be printed.
		*/
		static void LogInfo(const char* zMessage, ...);

		/*
		Sets which messages the log will output to the console, and
		which messages will be ignored.

		@param eLevel - LogLevel enum for what the console should print/ignore.
		*/
		static void SetLogLevel(LogLevel eLevel);

		// Deleted Functions ///////////////////////////////////////////////////

		Logger() = delete;

	private:
		static LogLevel _seLogLevel; // The current LogLevel the console prints.
	};
}

// Logging Macros //////////////////////////////////////////////////////////////
#define SoulLogError(message, ...) (Soul::Logger::LogError("[Error]: " message "\n", ##__VA_ARGS__))
#define SoulLogWarning(message, ...) (Soul::Logger::LogWarning("[Warning]: " message "\n", ##__VA_ARGS__))
#define SoulLogInfo(message, ...) (Soul::Logger::LogInfo("[Info]: " message "\n", ##__VA_ARGS__))