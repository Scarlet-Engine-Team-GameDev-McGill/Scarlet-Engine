#pragma once

// include signal.h to raise a debug break in POSIX systems (non-windows)
#ifndef _MSC_VER
#include <signal.h>
#endif

#ifdef _MSC_VER
#define PLATFORM_BREAK() __debugbreak();
#else
#define PLATFORM_BREAK() raise(SIGTRAP);
#endif

#ifdef DEBUG
#define check(pred)	\
{					\
	if (!(pred))	\
	{				\
		PLATFORM_BREAK();\
	}				\
}
#else
#define check(pred)
#endif