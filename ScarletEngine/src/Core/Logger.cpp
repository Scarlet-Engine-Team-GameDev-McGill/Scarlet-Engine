#include "Core/Logger.h"

namespace ScarletEngine
{
	Logger::~Logger()
	{
		if (LogFile)
		{
			fclose(LogFile);
		}
	}

	void Logger::Log(LogLevel Level, const char* Message) const
	{
		static const char* VerbosePrefix = "[Verbose] ";
		static const char* InfoPrefix = "[Info] ";
		static const char* WarningPrefix = "[Warn] ";
		static const char* ErrorPrefix = "[Error] ";
		static const char* DefaultPrefix = "";

		const char** PrefixPtr = nullptr;
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
		default:
			PrefixPtr = &DefaultPrefix;
			break;
		}

		printf("%s", *PrefixPtr);
		printf("%s", Message);
		printf("\n");

		// If compiling for Windows and in debug mode, 
		// we can print to the debug output (visible in Visual Studio Output tab)
#if defined(_WIN32) && defined(DEBUG)
		OutputDebugString(*PrefixPtr);
		OutputDebugString(Message);
		OutputDebugString("\n");
#endif
		if (LogFile)
		{
			time_t Now = time(0);
			struct tm* TimeStruct;
			char Buffer[80];
			TimeStruct = localtime(&Now);

			assert(TimeStruct != nullptr);
			strftime(Buffer, sizeof(Buffer), "[%Y-%m-%d.%X] ", TimeStruct);

			fprintf(LogFile, "%s", Buffer);
			fprintf(LogFile, "%s", *PrefixPtr);
			fprintf(LogFile, "%s", Message);
			fprintf(LogFile, "\n");
		}
	}

	void Logger::SetLogFile(const char* FilePath)
	{
		if (LogFile)
		{
			fclose(LogFile);
		}
		LogFile = fopen(FilePath, "w+");
	}
}