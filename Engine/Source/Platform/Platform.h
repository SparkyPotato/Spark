#pragma once

#ifdef ON_WINDOWS

using Char = wchar_t;
#define STRING(x) L ## x

#endif