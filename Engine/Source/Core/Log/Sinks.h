#pragma once

namespace Spark
{
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

	class ConsoleSink : public ILogSink
	{
	public:
		void PushLog(const Log& log) override;
	};

	class FileSink : public ILogSink
	{
	public:
		FileSink();

		void PushLog(const Log& log) override;

	private:
		FILE* m_File;
	};
}
