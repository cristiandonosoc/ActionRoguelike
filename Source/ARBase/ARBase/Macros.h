#pragma once

// This file contains macros that are useful to the rest of development.

// AR_CONCAT will concatenate two elements of the macros into one string.
// Useful for creating macro defined variable names. For example:
//
// AR_CONCAT(my_variable_name__, __LINE__) --> Results in my_variable_name__73
#define AR_CONCAT(x, y) AR_CONCAT_(x, y)
#define AR_CONCAT_(x, y) x##y

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