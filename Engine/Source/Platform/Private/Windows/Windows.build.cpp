// Copyright 2020 SparkyPotato

#include "SparkPCH.h"

// Only build on Windows
#ifdef ON_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h> // Just including Windows.h once for all platform related stuff

#include "Platform.cpp"
#include "Entry.cpp"

#include "WindowManager/WWindowManager.cpp"
#include "Thread/Mutex.cpp"

#endif 
