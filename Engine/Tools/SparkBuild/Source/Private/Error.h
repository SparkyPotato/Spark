/*
	SparkBuild.Private.Error
	Copyright 2020 SparkyPotato

	Error emitter
*/

#pragma once

#include "Globals.h"
#include "BasePlatform.h"

class Error
{
public:
	template <typename ...Args>
	Error(const String& errorCode, const Args&... args)
	{
		BasePlatform::Output("Error: ", errorCode, args...);
	}
};
