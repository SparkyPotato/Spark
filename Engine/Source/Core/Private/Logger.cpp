#include "Core/Log/Logger.h"

#include <iostream>

namespace Spark
{
	Array<LogSink*> Logger::m_RegisteredSinks;
	String Logger::m_FormatString = STRING("[%2d:%2d:%2d:%3d] %s (%s): ");

	String LogLevelToString(LogLevel level)
	{
		switch (level)
		{
		case LogLevel::Verbose: return STRING("Verbose");
		case LogLevel::Trace: return STRING("Trace");
		case LogLevel::Log: return STRING("Log");
		case LogLevel::Info: return STRING("Info");
		case LogLevel::Debug: return STRING("Debug");
		case LogLevel::Warning: return STRING("Warning");
		case LogLevel::Error: return STRING("Error");
		case LogLevel::Fatal: return STRING("Fatal");
		}

		return String();
	}

	void Logger::Init()
	{
	#ifdef IS_DEBUG
		PushSink(new DebugSink);
	#endif

	#ifdef IS_CONSOLE
		PushSink(new ConsoleSink);
	#endif
	}

	void Logger::Shutdown()
	{
		for (auto sink : m_RegisteredSinks)
		{
			delete sink;
		}
	}

	void Logger::PushSink(LogSink* sink) noexcept
	{
		m_RegisteredSinks.Add(sink);
	}

	void DebugSink::PushLog(const Log& log)
	{
		Platform::DebugOutput(log.FormattedMessage + STRING("\n"));
	}

	void ConsoleSink::PushLog(const Log& log)
	{
		std::wcout << log.FormattedMessage.GetCharPointer() << STRING("\n");
	}
}
