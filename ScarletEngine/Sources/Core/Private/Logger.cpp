#include "Logger.h"

#include <cassert>
#include <ctime>

#ifdef _WIN32
#include <Windows.h>
#endif


namespace ScarletEngine
{
	Logger::~Logger()
	{
		ZoneScoped
		if (LogFile)
		{
			fclose(LogFile);
		}
	}

	void Logger::Log(LogLevel Level, const char* Message)
	{
		ZoneScoped
		static const char* VerbosePrefix = "[Verbose]";
		static const char* InfoPrefix = "[Info]";
		static const char* WarningPrefix = "[Warn]";
		static const char* ErrorPrefix = "[Error]";
		static const char* DefaultPrefix = "";

		const char** PrefixPtr = &DefaultPrefix;
		switch (Level)
		{
		case LogLevel::LogVerbose:
			PrefixPtr = &VerbosePrefix;
			break;
		case LogLevel::LogInfo:
			PrefixPtr = &InfoPrefix;
			break;
		case LogLevel::LogWarning:
			PrefixPtr = &WarningPrefix;
			break;
		case LogLevel::LogError:
			PrefixPtr = &ErrorPrefix;
			break;
		}

		printf("%s %s\n", *PrefixPtr, Message);

		time_t Now = time(0);
		struct tm* TimeStruct;
		char TimeStringBuffer[80];
		TimeStruct = localtime(&Now);

		assert(TimeStruct != nullptr);
		strftime(TimeStringBuffer, sizeof(TimeStringBuffer), "[%Y-%m-%d.%X]", TimeStruct);

		// If compiling for Windows and in debug mode, 
		// we can print to the debug output (visible in Visual Studio Output tab)
#if defined(_WIN32) && defined(DEBUG)
		OutputDebugString(TimeStringBuffer);
		OutputDebugString(*PrefixPtr);
		OutputDebugString(Message);
		OutputDebugString("\n");
#endif
		if (LogFile)
		{
			fprintf(LogFile, "%s %s %s\n", TimeStringBuffer, *PrefixPtr, Message);
		}

		strftime(TimeStringBuffer, sizeof(TimeStringBuffer), "[%X]", TimeStruct);

		char Buffer[4096];
		snprintf(Buffer, 4096, "%s %s %s\n", TimeStringBuffer , *PrefixPtr, Message);

		OnMessageLogged.Broadcast(Level, Buffer);
	}

	void Logger::SetLogFile(const char* FilePath)
	{
		ZoneScoped
		if (LogFile)
		{
			fclose(LogFile);
		}
		LogFile = fopen(FilePath, "w+");
	}
}