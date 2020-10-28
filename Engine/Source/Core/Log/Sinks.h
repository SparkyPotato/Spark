// Copyright 2020 SparkyPotato

#pragma once

namespace Spark
{
	/*
		Sink for all log messages, just implement PushLog(),
		and add the sink to the logger by calling Logger::PushSink<T>(),
		where T is your derived class
	*/
	class ILogSink
	{
	public:
		virtual ~ILogSink() {}

		virtual void PushLog(const Log& log) = 0;
	};

	class DebugSink : public ILogSink 
	{
	public:
		void PushLog(const Log& log) override;
	};
}
