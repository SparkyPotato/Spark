#pragma once

#ifdef _WIN32
	#ifdef _WIN64
		#define ON_WINDOWS
	#else
		#error "Spark is 64-bit only!"
	#endif
#else
	#error "Windows is the only supported platform!"
#endif

namespace Spark
{
#ifdef ON_WINDOWS

	using Char = wchar_t;

	using uint = uint32_t;
	using uint8 = uint8_t;
	using uint16 = uint16_t;
	using uint32 = uint;
	using uint64 = uint64_t;

	using fuint = uint_fast32_t;
	using fuint8 = uint_fast8_t;
	using fuint16 = uint_fast16_t;
	using fuint32 = fuint;
	using fuint64 = uint_fast64_t;

	using int8 = int8_t;
	using int16 = int16_t;
	using int32 = int32_t;
	using int64 = int64_t;

	using fint8 = int_fast8_t;
	using fint16 = int_fast16_t;
	using fint32 = int_fast32_t;
	using fint64 = int_fast64_t;

	using byte = uint8;

#endif
}

#ifdef ON_WINDOWS
	#define STRING(x) L ## x

	#if IS_DEBUG
		#include <assert.h>
		#define SPARK_ASSERT(x) assert(x)
	#else
		#define SPARK_ASSERT(x)
	#endif
#endif

#define UNIMPLEMENTED(type) SPARK_ASSERT(false); return (type) 0

namespace Spark
{
	struct DateTime;
	class String;

namespace Platform
{
	DateTime GetSystemTime();
	void DebugOutput(const String& string);

	void ShowMessageBox(const String& title, const String& message);
}
}
