// Copyright 2020 SparkyPotato

#pragma once

namespace Spark
{
	class String;

	void Assert(String message, int line, String file);
}

#if IS_DEBUG
#define SPARK_ASSERT(x, message) if (!(x)) Spark::Assert(message, __LINE__, __FILE__)
#else
#define SPARK_ASSERT(x, message) 
#endif
