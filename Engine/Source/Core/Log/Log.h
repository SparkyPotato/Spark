#pragma once

namespace Spark
{
	enum class LogLevel
	{
		Verbose = 0,
		Trace = 1,
		Log = 2,
		Debug = 3,
		Info = 4,
		Warning = 5,
		Error = 6,
		Fatal = 7
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

// Define a log category usable in only one source file
#define DEFINE_LOG_CATEGORY_FILE(Category, Level) \
static struct Log##Category : public LogCategory<LogLevel::Level> \
{ \
	Log##Category() : LogCategory(STRING(#Category)) {} \
} Category;
