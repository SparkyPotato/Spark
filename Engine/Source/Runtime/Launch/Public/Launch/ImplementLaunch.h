// Launch.Public.ImplementLaunch
// Copyright 2020 SparkyPotato

// Must be included in the base C++ file of every executable module

#pragma once

#include "Launch/Launch.h"

extern int Launch_API EntryPoint();

int main()
{
	return EntryPoint();
}