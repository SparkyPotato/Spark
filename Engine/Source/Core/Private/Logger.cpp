// Copyright 2020 SparkyPotato

#include "Core/Log/Logger.h"

#include "Runtime/RunLoop/RunLoop.h"

namespace Spark
{
	Array<ILogSink*> Logger::m_RegisteredSinks = { snew DebugSink };
	String Logger::m_FormatString = STRING("[{:2}:{:2}:{:2}:{:3}] {} ({}): ");

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
		Platform::DebugOutput(log.FormattedMessage);
	}
}
