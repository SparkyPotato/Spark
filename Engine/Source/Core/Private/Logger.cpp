#include "Core/Log/Logger.h"

namespace Spark
{
	Array<ILogSink*> Logger::m_RegisteredSinks;
	String Logger::m_FormatString = STRING("[%02d:%02d:%02d:%03d] %s (%s): ");

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

	void Logger::PushSink(ILogSink* sink) 
	{
		m_RegisteredSinks.Add(sink);
	}

	void DebugSink::PushLog(const Log& log)
	{
		Platform::DebugOutput(log.FormattedMessage + STRING("\n"));
	}

	#define CL_DARKRED STRING("\x1b[31m")
	#define CL_RED STRING("\x1b[91m")
	#define CL_GREEN STRING("\x1b[92m")
	#define CL_YELLOW STRING("\x1b[93m")
	#define CL_MAGENTA STRING("\x1b[95m")
	#define CL_CYAN STRING("\x1b[96m")
	#define CL_WHITE STRING("\x1b[97m")
	#define CL_RESET STRING("\x1b[0m")

	void ConsoleSink::PushLog(const Log& log)
	{
		switch (log.Level)
		{
		case LogLevel::Verbose:
			wprintf(STRING("%s\n"), log.FormattedMessage.GetCharPointer());
			break;
		case LogLevel::Trace:
			wprintf(CL_WHITE STRING("%s\n") CL_RESET, log.FormattedMessage.GetCharPointer());
			break;
		case LogLevel::Log:
			wprintf(CL_GREEN STRING("%s\n") CL_RESET, log.FormattedMessage.GetCharPointer());
			break;
		case LogLevel::Info:
			wprintf(CL_CYAN STRING("%s\n") CL_RESET, log.FormattedMessage.GetCharPointer());
			break;
		case LogLevel::Debug:
			wprintf(CL_MAGENTA STRING("%s\n") CL_RESET, log.FormattedMessage.GetCharPointer());
			break;
		case LogLevel::Warning:
			wprintf(CL_YELLOW STRING("%s\n") CL_RESET, log.FormattedMessage.GetCharPointer());
			break;
		case LogLevel::Error:
			wprintf(CL_RED STRING("%s\n") CL_RESET, log.FormattedMessage.GetCharPointer());
			break;
		case LogLevel::Fatal:
			wprintf(CL_DARKRED STRING("%s\n") CL_RESET, log.FormattedMessage.GetCharPointer());
			break;
		}
	}
}
