// Copyright 2020 SparkyPotato

// Pre-compiled Header for ONLY engine use. If you are writing a module for Spark
// or a game with the engine, don't you dare include it.

#include <initializer_list> // Only way to use initializer lists in our custom containers
#include <new> // Required for placement-new usage

#include "Platform/Platform.h"

#include "Core/Types/String.h"
#include "Core/Types/Time.h"
#include "Core/Types/Container.h"
#include "Core/Types/Math.h"

#include "Core/Memory/Memory.h"

#include "Core/IO/IO.h"
#include "Core/IO/Formatter.h"

#include "Core/Log/Logger.h"

#include "Core/Types/Pointer.h"

#include "Core/Object/ClassManager.h"

#include "Core/Thread/Mutex.h"
