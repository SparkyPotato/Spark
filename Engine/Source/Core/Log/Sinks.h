#pragma once

namespace Spark
{
	class LogSink
	{
	public:
		virtual void PushLog(const Log& log) = 0;
	};

	class DebugSink : public LogSink 
	{
	public:
		void PushLog(const Log& log) override;
	};
}
