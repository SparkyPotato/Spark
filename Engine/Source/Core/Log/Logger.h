// Copyright 2020 SparkyPotato

#pragma once
#include "Core/Log/Log.h"
#include "Core/Log/Sinks.h"

namespace Spark
{
	class Logger
	{
	public:
		static void Shutdown();

		template<LogLevel T, typename ...Args>
		static void DoLog(LogCategory<T>* category, LogLevel level, const String& format, const Args&... args);

		static void HandleFatal();

		// Push a custom log sink into the logger
		template<typename T>
		static void PushSink()
		{
			m_RegisteredSinks.Add(snew T);
		}

	private:
		static Array<ILogSink*> m_RegisteredSinks;
		static String m_FormatString;
	};

	template<LogLevel T, typename ...Args>
	void Logger::DoLog(LogCategory<T>* category, LogLevel level, const String& log, const Args&... args)
	{
		if (level < T) return;

		DateTime now = DateTime::Now();

		Log logStruct
		(
			category->GetCategoryName(), 
			level, 
			IO::FormatString
			(
				m_FormatString + log,
				now.Hour, now.Minute, now.Second, now.Millisecond, 
				category->GetCategoryName(),
				LogLevelToString(level), 
				args...
			),
			now
		);

		for (auto sink : m_RegisteredSinks)
		{
			sink->PushLog(logStruct);
		}

		if (level == LogLevel::Fatal)
		{
			HandleFatal();
		}
	}
}

#define SPARK_LOG(Category, Level, ...) \
Spark::Logger::DoLog(&Category, Spark::LogLevel::Level, __VA_ARGS__)
