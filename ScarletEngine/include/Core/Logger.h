#pragma once

#include <cstdio>
#include <cassert>
#include <ctime>

#ifdef _WIN32
#include <Windows.h>
#endif

enum class LogLevel
{
	LogVerbose,
	LogInfo,
	LogWarning,
	LogError
};

namespace ScarletEngine
{
	class Logger
	{
	public:
		template <typename ...Args>
		inline void Log(LogLevel Level, const char* Message, const Args&... args)
		{
			char* Prefix = nullptr;
			switch (Level)
			{
			case LogLevel::LogVerbose:
				Prefix = "[Verbose] ";
				break;
			case LogLevel::LogInfo:
				Prefix = "[Info] ";
				break;
			case LogLevel::LogWarning:
				Prefix = "[Warn] ";
				break;
			case LogLevel::LogError:
				Prefix = "[Error] ";
				break;
			default:
				Prefix = "";
				break;
			}
			
			printf(Prefix);
			printf(Message, args...);
			printf("\n");
	
			// If compiling for Windows and in debug mode, 
			// we can print to the debug output (visible in Visual Studio Output tab)
#if defined(_WIN32) && defined(DEBUG)
			OutputDebugString(Prefix);
			OutputDebugString(Message, Args...);
			OutputDebugString("\n");
#endif
			if (LogFile)
			{
				time_t Now = time(0);
				struct tm TimeStruct;
				char Buffer[80];
				localtime_s(&TimeStruct, &Now);
				strftime(Buffer, sizeof(Buffer), "[%Y-%m-%d.%X] ", &TimeStruct);

				fprintf(LogFile, Buffer);
				fprintf(LogFile, Prefix);
				fprintf(LogFile, Message, args...);
				fprintf(LogFile, "\n");
			}
		}

		// #todo: Should be set by some config file during engine initialization
		void SetLogFile(const char* FilePath)
		{
			if (LogFile)
			{
				fclose(LogFile);
			}
			fopen_s(&LogFile, FilePath, "w+");
		}

		static Logger& Get() { static Logger Instance; return Instance; }

		~Logger()
		{
			if (LogFile)
			{
				fclose(LogFile);
			}
		}
	private:
		FILE* LogFile = nullptr;
	};
}

#define SCAR_LOG(Level, ...) ScarletEngine::Logger::Get().Log(LogLevel::##Level, __VA_ARGS__)