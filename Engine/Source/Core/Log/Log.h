#pragma once

namespace Spark
{
	enum class LogLevel
	{
		Verbose = 0,
		Trace = 1,
		Debug = 2,
		Info = 3,
		Warning = 4,
		Error = 5,
		Fatal = 6
	};

	struct Log
	{
		String CategoryName;
		LogLevel Level;
		String FormattedMessage;
		DateTime Time;
	};

	template<LogLevel LeastLevel>
	class LogCategory
	{
	public:
		LogCategory(String Name)
			: m_CategoryName(Name)
		{}

		inline bool ShouldLog(LogLevel level) { return level >= LeastLevel; }

		const String& GetCategoryName() { return m_CategoryName; }

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

// Define a log category usable in only one source file
#define DEFINE_LOG_CATEGORY_FILE(Category, Level) \
static struct Log##Category : public LogCategory<LogLevel::Level> \
{ \
	Log##Category() : LogCategory(STRING(#Category)) {} \
} Category;
