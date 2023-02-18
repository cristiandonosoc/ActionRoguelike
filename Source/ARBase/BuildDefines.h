#pragma once

// We define our set of defines in which we control where the code is build.
// This is to avoid using the improper build define.

// clang-format off
#define AR_BUILD_EDITOR UE_EDITOR					// Editor build only.
#define AR_BUILD_SERVER UE_SERVER					// Dedicated server (NOT listen server).
#define AR_BUILD_CLIENT (UE_CLIENT || UE_EDITOR)	// Client & Editor builds.

#define AR_BUILD_GAME_ONLY (UE_GAME && !UE_CLIENT)	// Game only, excluding client builds.
#define AR_BUILD_FINAL (UE_BUILD_SHIPPING || UE_BUILD_TEST)		// Shipping & test builds.
#define AR_BUILD_DEBUG (!AR_BUILD_FINAL)			// Non-final builds.
#define AR_BUILD_AUTOTEST (!AR_BUILD_FINAL)			// ANy build that supports tests.

// #define AR_HAS_CLIENT_CODE (UE_CLIENT || UE_EDITOR || UE_GAME || IS_PROGRAM)
// #define AR_HAS_SERVER_CODE (UE_SERVER || UE_EDITOR || (UE_GAME && !UE_CLIENT) || IS_PROGRAM)

// Cross-platform optimization pragmas.

#if PLATFORM_WINDOWS

#define AR_OPTIMIZATION_ON  __pragma(optimize("", on))
#define AR_OPTIMIZATION_OFF __pragma(optimize("", off))

#elif PLATFORM_LINUX

#define AR_OPTIMIZATION_ON  _Pragma("clang optimize on")
#define AR_OPTIMIZATION_OFF _Pragma("clang optimize off")

#else

#error "Unsupported platform"

#endif

// clang-format on
