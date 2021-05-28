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
/**
 * Breaks the debugger if the predicate evaluates to false and then aborts the program.
 * Essentially acts like an assert with a debug break.
 *
 * Only active in debug builds
 */
#define check(pred)	\
{					\
	if (!(pred))	\
	{				\
		PLATFORM_BREAK();\
		abort();	\
	}				\
}
#else
/**
* Breaks the debugger if the predicate evaluates to false and then aborts the program.
* Essentially acts like an assert with a debug break.
*
* Only active in debug builds
*/
#define check(pred) (void)(pred)
#endif

/** Define OUT as nothing, it is simply used to markup out params in code to make it more readable */
#define OUT

#define NODISCARD [[nodiscard]]