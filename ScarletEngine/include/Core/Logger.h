#pragma once

#include <cstdio>
#include <cassert>
#include <ctime>

#ifdef _WIN32
#include <Windows.h>
#endif


namespace ScarletEngine
{
	enum class LogLevel
	{
		LogVerbose,
		LogInfo,
		LogWarning,
		LogError
	};

	class Logger
	{
	public:
		void Log(LogLevel Level, const char* Message) const;

		void SetLogFile(const char* FilePath);

		static Logger& Get() { static Logger Instance; return Instance; }

		~Logger();
	private:
		FILE* LogFile = nullptr;
	};
}

#define SCAR_LOG(Level, Format, ...)		\
{											\
	constexpr int BufferSize = 4096;		\
	char Buffer[BufferSize];				\
	snprintf(Buffer, BufferSize, Format, ##__VA_ARGS__);\
	ScarletEngine::Logger::Get().Log(ScarletEngine::LogLevel::Level, Buffer);\
}
