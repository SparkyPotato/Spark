#pragma once

namespace Spark
{
	class Logger
	{
	public:
		static void Init();
		static void Shutdown();

	private:

	};
}

#define SPARK_LOG(Category, Level, Text, ...)
