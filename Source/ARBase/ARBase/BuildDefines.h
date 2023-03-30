pragma once

// Build Defines.
// -------------------------------------------------------------------------------------------------
// We define our set of defines in which we control where the code is build.
// This is to avoid using the improper build define.

// clang-format off
// Editor build only.
#define AR_BUILD_EDITOR UE_EDITOR					
// Dedicated server (NOT listen server).
#define AR_BUILD_DEDICATED_SERVER UE_SERVER
// This is if there is server code to be compiled.
#define AR_BUILD_SERVER WITH_SERVER_CODE
// Parts that are client only (including editor).
#define AR_BUILD_CLIENT ((!UE_SERVER && !WITH_SERVER_CODE) || UE_EDITOR)	

#define AR_BUILD_GAME_ONLY UE_GAME 
#define AR_BUILD_FINAL (UE_BUILD_SHIPPING || UE_BUILD_TEST)		// Shipping & test builds.
#define AR_BUILD_DEBUG (!AR_BUILD_FINAL)			// Non-final builds.
#define AR_BUILD_AUTOTEST (!AR_BUILD_FINAL)			// ANy build that supports tests.

// #define AR_HAS_CLIENT_CODE (UE_CLIENT || UE_EDITOR || UE_GAME || IS_PROGRAM)
// #define AR_HAS_SERVER_CODE (UE_SERVER || UE_EDITOR || (UE_GAME && !UE_CLIENT) || IS_PROGRAM)
// clang-format off

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