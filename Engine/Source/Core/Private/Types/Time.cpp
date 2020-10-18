// Copyright 2020 SparkyPotato

#include "Core/Types/Time.h"

namespace Spark
{
	String DateTime::ToString() const 
	{
		return String();
	}

	DateTime DateTime::Now()
	{
		return Platform::GetSystemTime();
	}
}
