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

/** Breaks the debugger if the predicate evaluates to false. Only active in debug builds. */
#define ensure(pred)	\
{						\
	if (!(pred))		\
	{					\
		PLATFORM_BREAK();\
	}					\
}

#else
/**
* Breaks the debugger if the predicate evaluates to false and then aborts the program.
* Essentially acts like an assert with a debug break.
*
* Only active in debug builds
*/
#define check(pred) (void)(pred)

#define ensure(pred) (void)(pred)
#endif

/** Define OUT as nothing, it is simply used to markup out params in code to make it more readable */
#define OUT

/** Indicate to the compiler that the return value of this function should not be discarded. */ 
#define NODISCARD [[nodiscard]]

// These attributes are only defined in cpp 20 builds
#if __cplusplus >= 202002L
/** Indicate to the compiler that this case is likely to evaluate to true */
#define LIKELY [[likely]]
/** Indicate to the compiler that this case is unlikely to evaluate to true */ 
#define UNLIKELY [[unlikely]]
#else
/** Indicate to the compiler that this case is likely to evaluate to true */
#define LIKELY
/** Indicate to the compiler that this case is unlikely to evaluate to true */ 
#define UNLIKELY
#endif // __cplusplus >= 202002L