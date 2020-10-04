#include "Core/Types/Time.h"

namespace Spark
{
	String DateTime::ToString() const noexcept
	{
		return String();
	}

	DateTime DateTime::Now()
	{
		return Platform::GetCurrentSystemTime();
	}
}
