#pragma once

// Build Defines.
// -------------------------------------------------------------------------------------------------
// We define our set of defines in which we control where the code is build.
// This is to avoid using the improper build define.

// clang-format off
// Editor build only.
#define AR_BUILD_EDITOR UE_EDITOR

#ifndef AR_BUILD_CLIENT
#define AR_BUILD_CLIENT 0
#endif

#ifndef AR_BUILD_SERVER
#define AR_BUILD_SERVER 0
#endif

// Dedicated client (NOT listen server)
#define AR_BUILD_DEDICATED_CLIENT AR_BUILD_CLIENT && !AR_BUILD_SERVER
// Dedicated server (NOT listen server).
#define AR_BUILD_DEDICATED_SERVER !AR_BUILD_CLIENT && AR_BUILD_SERVER

#define AR_BUILD_GAME_ONLY UE_GAME 
#define AR_BUILD_FINAL (UE_BUILD_SHIPPING || UE_BUILD_TEST)		// Shipping & test builds.
#define AR_BUILD_DEBUG (!AR_BUILD_FINAL)			// Non-final builds.
#define AR_BUILD_AUTOTEST (!AR_BUILD_FINAL)			// ANy build that supports tests.
// clang-format on

// Cross-platform macros/pragmas.
// -------------------------------------------------------------------------------------------------

// clang-format off
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