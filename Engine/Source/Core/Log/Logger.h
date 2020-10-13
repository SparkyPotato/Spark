#pragma once
#include "Core/Log/Log.h"
#include "Core/Log/Sinks.h"

namespace Spark
{
	class Logger
	{
	public:
		static void Initialize();
		static void Shutdown();

		template<LogLevel T, typename ...Args>
		static void DoLog(LogCategory<T>* category, LogLevel level, const String& format, Args... args);

		static void HandleFatal();

		static void PushSink(ILogSink* sink);

	private:
		static Array<ILogSink*> m_RegisteredSinks;
		static String m_FormatString;
	};

	template<LogLevel T, typename ...Args>
	void Logger::DoLog(LogCategory<T>* category, LogLevel level, const String& format, Args... args)
	{
		if (!category->ShouldLog(level)) return;

		DateTime now = DateTime::Now();

		String temp = Formatter::Format(m_FormatString + format,
			now.Hour, now.Minute, now.Second, now.Millisecond, category->GetCategoryName().GetCharPointer(), 
			LogLevelToString(level).GetCharPointer(), args...
		);

		Log log(category->GetCategoryName(), level, temp, now);

		for (auto sink : m_RegisteredSinks)
		{
			sink->PushLog(log);
		}

		if (level == LogLevel::Fatal)
		{
			HandleFatal();
		}
	}
}

#define SPARK_LOG(Category, Level, Text, ...) \
Spark::Logger::DoLog(&Category, Spark::LogLevel::Level, Text, __VA_ARGS__)
