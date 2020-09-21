#pragma once

#ifndef SPARK_CORE
#define SPARK_CORE

#ifdef _WIN32
	#ifdef _WIN64
		#define ON_WINDOWS
	#else
		#error "Spark is 64-bit only!"
	#endif
#else
	#error "Windows is the only supported platform!"
#endif

#endif
