// Copyright 2020 SparkyPotato

#pragma once

namespace Spark
{
	class TimePoint
	{

	};

	struct DateTime
	{
		uint16 Year;
		uint16 Month;
		uint16 Day;
		uint16 Hour;
		uint16 Minute;
		uint16 Second;
		uint16 Millisecond;

		String ToString() const;

		static DateTime Now();
	};
}
