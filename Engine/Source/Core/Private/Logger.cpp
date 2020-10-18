// Copyright 2020 SparkyPotato

#include "Core/Log/Logger.h"

#include "Runtime/RunLoop/RunLoop.h"

namespace Spark
{
	Array<ILogSink*> Logger::m_RegisteredSinks = { new DebugSink, new ConsoleSink, new FileSink };
	String Logger::m_FormatString = STRING("[%02d:%02d:%02d:%03d] %s (%s): ");

	const String& LogLevelToString(LogLevel level)
	{
		// Prevent creation of temporaries for every single log
		static const String verbose = STRING("Verbose");
		static const String trace = STRING("Trace");
		static const String log = STRING("Log");
		static const String info = STRING("Info");
		static const String debug = STRING("Debug");
		static const String warn = STRING("Warning");
		static const String error = STRING("Error");
		static const String fatal = STRING("Fatal");

		switch (level)
		{
		case LogLevel::Verbose: return verbose;
		case LogLevel::Trace: return trace;
		case LogLevel::Log: return log;
		case LogLevel::Info: return info;
		case LogLevel::Debug: return debug;
		case LogLevel::Warning: return warn;
		case LogLevel::Error: return error;
		case LogLevel::Fatal: return fatal;
		}

		return log;
	}

	void Logger::Shutdown()
	{
		while (m_RegisteredSinks.Size() != 0)
		{
			delete m_RegisteredSinks[0];
			m_RegisteredSinks.Erase(0);
		}
	}

	void Logger::HandleFatal()
	{
		GRunLoop.ForceQuit();
	}

	void DebugSink::PushLog(const Log& log)
	{
		Platform::DebugOutput(log.FormattedMessage + STRING('\n'));
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

	FileSink::FileSink()
	{
		fopen_s(&m_File, "Log.txt", "w");
	}

	FileSink::~FileSink()
	{
		fclose(m_File);
	}

	void FileSink::PushLog(const Log& log)
	{
		fwprintf_s(m_File, STRING("%s\n"), log.FormattedMessage.GetCharPointer());
	}
}
