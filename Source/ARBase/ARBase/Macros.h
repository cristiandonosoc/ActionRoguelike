#pragma once

// This file contains macros that are useful to the rest of development.

// AR_CONCAT will concatenate two elements of the macros into one string.
// Useful for creating macro defined variable names. For example:
//
// AR_CONCAT(my_variable_name__, __LINE__) --> Results in my_variable_name__73
#define AR_CONCAT(x, y) __AR_CONCAT_IMPL(x, y)
#define __AR_CONCAT_IMPL(x, y) x##y

// AR_CONCAT3 works same as AR_CONCAT, only that supports 3 elements.
#define AR_CONCAT3(x, y, z) __AR_CONCAT3_IMPL(x, y, z)
#define __AR_CONCAT3_IMPL(x, y, z) x##y##z

// clang-format off
#if defined(__has_builtin)
	#if __has_builtin(__debugtrap)
		#define AR_DEBUGTRAP() __debugtrap()
	#endif
#elif defined(_MSC_VER)
	#define AR_DEBUGTRAP() __debugbreak()
#endif
// clang-format on

#ifndef AR_DEBUGTRAP
#error "Unsupported compiler!"
#endif // AR_DEBUG_TRAP