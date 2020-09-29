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

#endif
}

#ifdef ON_WINDOWS
	#define STRING(x) L ## x

	#if IS_DEBUG
		#include <assert.h>
		#define SPARK_ASSERT(x) do { assert(x); __debugbreak(); } while(false)
	#endif
#endif

#define UNIMPLEMENTED(type) SPARK_ASSERT(false); return (type) 0
