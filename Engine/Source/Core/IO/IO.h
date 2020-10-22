// Copyright 2020 SparkyPotato

#pragma once
#include <stdio.h>

namespace Spark
{
namespace IO
{
	using File = FILE*;

	File CreateFile(const String& file);
	File OpenFile(const String& file);
	void CloseFile(File file);
}
}
