// Copyright 2020 SparkyPotato

#include "Core/Log/Logger.h"

#include "Runtime/RunLoop/RunLoop.h"

namespace Spark
{
	Array<ILogSink*> Logger::m_RegisteredSinks = { snew DebugSink, snew ConsoleSink, snew FileSink };
	String Logger::m_FormatString = STRING("[%02d:%02d:%02d:%03d] %s (%s): ");

	// Prevent creation of temporaries for every single log
	static const String s_Verbose = STRING("Verbose");
	static const String s_Trace = STRING("Trace");
	static const String s_Log = STRING("Log");
	static const String s_Info = STRING("Info");
	static const String s_Debug = STRING("Debug");
	static const String s_Warn = STRING("Warning");
	static const String s_Error = STRING("Error");
	static const String s_Fatal = STRING("Fatal");

	const String& LogLevelToString(LogLevel level)
	{
		switch (level)
		{
		case LogLevel::Verbose: return s_Verbose;
		case LogLevel::Trace: return s_Trace;
		case LogLevel::Log: return s_Log;
		case LogLevel::Info: return s_Info;
		case LogLevel::Debug: return s_Debug;
		case LogLevel::Warning: return s_Warn;
		case LogLevel::Error: return s_Error;
		case LogLevel::Fatal: return s_Fatal;
		}

		return s_Log;
	}

	void Logger::Shutdown()
	{
		while (m_RegisteredSinks.Size() != 0)
		{
			sdelete m_RegisteredSinks[0];
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
