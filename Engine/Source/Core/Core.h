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

#if IS_DEBUG
#include <assert.h>
#define SPARK_ASSERT(x) do { assert(x); __debugbreak(); } while(false)
#endif

#endif

#define UNIMPLEMENTED SPARK_ASSERT(false); return 0
#define UNIMPLEMENTED_VOID SPARK_ASSERT(false)
