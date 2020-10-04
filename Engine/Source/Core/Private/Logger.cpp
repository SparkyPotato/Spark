#include "Core/Log/Logger.h"

namespace Spark
{
	Array<LogSink*> Logger::m_RegisteredSinks;

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
		StringStream temp;
		temp << STRING("[") << log.Time.Hour << STRING(":") << log.Time.Minute << STRING(":") << log.Time.Second << STRING(":") << log.Time.Millisecond << STRING("] "); // Time
		temp << log.CategoryName; // Category Name
		temp << STRING(" (") << LogLevelToString(log.Level) << STRING(") "); // Log Level
		temp << log.FormattedMessage << STRING("\n"); // Message

		Platform::DebugOutput(temp.GetString());
	}
}
