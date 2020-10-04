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
		Fatal = 7
	};

	String LogLevelToString(LogLevel level);

	struct Log
	{
		Log(const String& catName, LogLevel level, const String& message, const DateTime& time)
			: CategoryName(catName), Level(level), FormattedMessage(message), Time(time)
		{}

		String CategoryName;
		LogLevel Level;
		String FormattedMessage;
		DateTime Time;
	};

	template<LogLevel LeastLevel>
	class LogCategory
	{
	public:
		explicit LogCategory(String Name)
			: m_CategoryName(Name)
		{}

		inline bool ShouldLog(LogLevel level) { return level >= LeastLevel; }

		const String& GetCategoryName() const { return m_CategoryName; }

	private:
		String m_CategoryName;
	};
}

// Define a log category that should exist in a header file, accessible by anyone who includes the header
#define DECLARE_LOG_CATEGORY(Category, Level) \
extern struct Log##Category : public LogCategory<LogLevel::Level> \
{ \
	Log##Category() : LogCategory(STRING(#Category)) {} \
} Category;

// Define the log category that was declared in the header file. Only use once in a source file
#define DEFINE_LOG_CATEGORY(Category) \
Log##Category Category;

// Define a log category usable in only one source file (or unity build unit!)
#define DEFINE_LOG_CATEGORY_FILE(Category, Level) \
static struct Log##Category : public LogCategory<LogLevel::Level> \
{ \
	Log##Category() : LogCategory(STRING(#Category)) {} \
} Category;
