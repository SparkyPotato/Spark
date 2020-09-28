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

#ifdef ON_WINDOWS

#if !IS_DEBUG
#define NDEBUG
#endif
#include <assert.h>
#define SPARK_ASSERT(x) assert(x); __debugbreak()

#endif
