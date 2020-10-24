// Copyright 2020 SparkyPotato

#pragma once

namespace Spark
{
	enum class LogLevel
	{
		Verbose = 0,
		Trace = 1,
		Log = 2,
		Info = 3,
		Debug = 4,
		Warning = 5,
		Error = 6,
		Fatal = 7 // Causes the Engine to force quit immediately if logged
	};

	const String&LogLevelToString(LogLevel level);

	struct Log
	{
		Log(const String& catName, LogLevel level, String&& message, const DateTime& time)
			: CategoryName(catName), Level(level), FormattedMessage(static_cast<String&&>(message)), Time(time)
		{}

		const String& CategoryName; // Name of Category
		LogLevel Level; // Log Level
		String FormattedMessage; // Entire formatted message which just needs to be output
		DateTime Time; // Time of log
	};

	template<LogLevel LeastLevel>
	class LogCategory
	{
	public:
		explicit LogCategory(String Name)
			: m_CategoryName(Name)
		{}

		inline const String& GetCategoryName() const { return m_CategoryName; }

	private:
		String m_CategoryName;
	};
}

// Define a log category that should exist in a header file, accessible by anyone who includes the header
#define DECLARE_LOG_CATEGORY(Category, Level) \
extern struct Log##Category : public Spark::LogCategory<Spark::LogLevel::Level> \
{ \
	Log##Category() : LogCategory(STRING(#Category)) {} \
} Category;

// Define the log category that was declared in the header file. Only use once in a source file
#define DEFINE_LOG_CATEGORY(Category) \
Log##Category Category;

// Define a log category usable in only one source file (or unity build unit!)
#define DEFINE_LOG_CATEGORY_FILE(Category, Level) \
static struct Log##Category : public Spark::LogCategory<Spark::LogLevel::Level> \
{ \
	Log##Category() : LogCategory(STRING(#Category)) {} \
} Category;
