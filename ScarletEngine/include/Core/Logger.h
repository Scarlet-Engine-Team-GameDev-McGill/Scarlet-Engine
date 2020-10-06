#pragma once

#include <cstdio>
#include "Event.h"

namespace ScarletEngine
{
	enum class LogLevel
	{
		LogVerbose,
		LogInfo,
		LogWarning,
		LogError
	};

	using OnMessageLoggedEvent = Event<LogLevel, const char*>;

	class Logger
	{
	public:
		void Log(LogLevel Level, const char* Message);

		void SetLogFile(const char* FilePath);

		static Logger& Get() { static Logger Instance; return Instance; }

		const OnMessageLoggedEvent& GetOnMessageLogged() const { return OnMessageLogged; }

	private:
		Logger() = default;
		Logger(const Logger&) = delete;
		Logger(Logger&&) = delete;
		~Logger();

		FILE* LogFile = nullptr;
		OnMessageLoggedEvent OnMessageLogged;
	};
}

#define SCAR_LOG(Level, Format, ...)		\
{											\
    ZoneScoped								\
	constexpr int BufferSize = 4096;		\
	char Buffer[BufferSize];				\
	snprintf(Buffer, BufferSize, Format, ##__VA_ARGS__);\
	ScarletEngine::Logger::Get().Log(ScarletEngine::LogLevel::Level, Buffer);\
}
