// Copyright 2020 SparkyPotato

#ifdef _WIN32
	#ifdef _WIN64
		#define PLATFORM_WINDOWS
	#else
		#error 32-bit is not supported!
	#endif
#endif
