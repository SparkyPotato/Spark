#pragma once
#include "Core/Log/Log.h"
#include <iostream>

namespace Spark
{
	class LogSink
	{
	public:
		virtual void PushLog(const Log& log) = 0;
	};

	class Logger
	{
	public:
		static void Init();
		static void Shutdown();

		template<LogLevel T, typename ...Args>
		static void DoLog(LogCategory<T>* category, LogLevel level, String format, Args... args);

		static void PushSink(LogSink* sink) noexcept;

	private:
		static Array<LogSink*> m_RegisteredSinks;
	};

	template<LogLevel T, typename ...Args>
	void Logger::DoLog(LogCategory<T>* category, LogLevel level, String format, Args... args)
	{
		if (!category->ShouldLog(level)) return;

		Log log;
		log.CategoryName = category->GetCategoryName();
		log.Level = level;
		log.FormattedMessage = Formatter::Format(format, args...);

		for (auto sink : m_RegisteredSinks)
		{
			sink->PushLog(log);
		}
	}
}

#define SPARK_LOG(Category, Level, Text, ...) \
Logger::DoLog(&Category, LogLevel::##Level, Text, __VA_ARGS__);
