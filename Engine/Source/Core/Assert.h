// Copyright 2020 SparkyPotato

#pragma once

namespace Spark
{
	class String;

	void Assert(const String& message, int line, const char* file);
}

#if IS_DEBUG
	#define SPARK_ASSERT(x, message) if (!(x)) Spark::Assert(message, __LINE__, __FILE__)
#else
	#define SPARK_ASSERT(x, message) 
#endif
