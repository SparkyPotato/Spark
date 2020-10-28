// Copyright 2020 SparkyPotato

#pragma once
#include <stdint.h>

#include "Core/Assert.h"

#ifdef _WIN32
	#ifdef _WIN64
		#define ON_WINDOWS
	#else
		#error "Spark is 64-bit only!"
	#endif
#elif defined (__APPLE__) || defined (__MACH__)
    #include <TargetConditionals.h>
    #if TARGET_OS_MAC == 1
        #define ON_MAC
    #endif
#else
	#error Platform not supported!
#endif

namespace Spark
{
#ifdef ON_WINDOWS
	using Char = wchar_t;
#elif defined (ON_MAC)
	using Char = char8_t;
#endif

	using uint = uint32_t;
	using uint8 = uint8_t;
	using uint16 = uint16_t;
	using uint32 = uint32_t;
	using uint64 = uint64_t;

	using fuint = uint_fast32_t;
	using fuint8 = uint_fast8_t;
	using fuint16 = uint_fast16_t;
	using fuint32 = uint_fast32_t;
	using fuint64 = uint_fast64_t;

	using int8 = int8_t;
	using int16 = int16_t;
	using int32 = int32_t;
	using int64 = int64_t;

	using fint = int_fast32_t;
	using fint8 = int_fast8_t;
	using fint16 = int_fast16_t;
	using fint32 = int_fast32_t;
	using fint64 = int_fast64_t;

	using byte = uint8;

	struct DateTime;
	class String;

	namespace Platform
	{
		void RegisterClasses();

		DateTime GetSystemTime();
		void DebugOutput(const String& string);

		void ShowMessageBox(const String& title, const String& message);
	
		void Print(const String& string);

#ifdef ON_WINDOWS
		Char* ToUnicode(const char* ascii);
#endif
	}

#ifdef ON_WINDOWS
	class Mutex
	{
	public:
		Mutex();
		~Mutex();

		void Lock();
		void Unlock();

	private:
		void* m_Handle;
	};
#endif
}

#ifdef ON_WINDOWS
	#define STRING(x) L ## x
#elif defined (ON_MAC)
	#define STRING(x) u8 ## x
#endif

#define UNIMPLEMENTED(type) SPARK_ASSERT(false, STRING("Unimplemented function called!")); return (type) 0

#define snew new(__FILE__, __LINE__)
#define sdelete delete

#define MemAlloc(size) Spark::Memory::Alloc(size, __FILE__, __LINE__)
#define MemFree(pointer) Spark::Memory::Dealloc(pointer)
