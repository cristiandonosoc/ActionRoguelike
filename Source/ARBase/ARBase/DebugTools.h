#pragma once

#include <ARBase/BuildDefines.h>
#include <ARBase/Macros.h>
#include <ARBase/NotNullPtr.h>

#include <Logging/LogMacros.h>

class UWorld;

// AR_DEBUG_DRAW_IMPLEMENTATION is a macro we use to define whether these debug functions are to be
// compiled in, or they should just be ignored by the compiler (aka. empty stubs).
//
// That way normal code can just call DebugDraw calls and not care whether we are on release or not.

// clang-format off
#if AR_BUILD_DEBUG
#define AR_DEBUG_DRAW_ENABLED 1
#define AR_DEBUG_DRAW_IMPLEMENTATION ;
#else
#define AR_DEBUG_DRAW_ENABLED 0
#define AR_DEBUG_DRAW_IMPLEMENTATION {}
#endif // AR_BUILD_DEBUG
// clang-format on

// DECLARE_DEBUG_CATEGORY declares a new category id for our debug system, as well as an UE log
// category.
//
// IMPORTANT: Any category declared here has to have the equivalent AR_DEFINE_DEBUG_CATEGORY call
//            within the .cpp!
#define DECLARE_DEBUG_CATEGORY(ns, dllspec, category_name, category_variable,                      \
							   ue_default_verbosity, ue_compile_time_verbosity)                    \
	namespace ns                                                                                   \
	{                                                                                              \
	static constexpr uint32 category_variable = UNIQUE_CATEGORY_ID;                                \
	}                                                                                              \
	dllspec DECLARE_LOG_CATEGORY_EXTERN(category_name, ue_default_verbosity,                       \
										ue_compile_time_verbosity);

// AR_DEFINE_DEBUG_CATEGORY registers a new category so that code can enable/disable the category
// on runtime in a convenient manner.
//
// This way code can just call always debug draw from the code and they will only run if the
// category is enabled.
//
// All of this code is automatically stripped out in release builds.
//
// IMPORTANT: |category_name| has to a something that can be "stringified" into a variable name.
//            This is to ensure that we can get unique variables across names (as long as they don't
//            collide on name and line number).
//            Basically, just use something that can be used as a C++ variable name.
#define AR_DEFINE_DEBUG_CATEGORY(category, log_category_name, console_category_name,               \
								 default_enabled, description)                                     \
	static ::debug::__DebugCategoryRegisterer AR_CONCAT3(__debug_draw_category_registerer__,       \
														 category_name, __LINE__)(                 \
		category, #console_category_name, default_enabled, description, __FILE__, __LINE__);       \
	DEFINE_LOG_CATEGORY(log_category_name);

// ARDebugDraw represents all the debug draw capabilities that are automatically stripped out in
// release builds, so gameplay code can always call.

// Define base categories.
// For the unique category id, we add a big offset to differentiate from the game categories.
#define UNIQUE_CATEGORY_ID __LINE__ + 0xf000'0000
DECLARE_DEBUG_CATEGORY(ar, ARBASE_API, LogARBase_Messaging, BASE_MESSAGING, Log, All);
#undef UNIQUE_CATEGORY_ID

namespace debug
{

// clang-format off
#if AR_DEBUG_DRAW_ENABLED
bool ARBASE_API IsCategoryEnabled(int32 category);
#else
FORCEINLINE static bool IsCategoryEnabled(int32 category) { return false; }
#endif // AR_DEBUG_DRAW_ENABLED
// clang-format on

// Category Functions ------------------------------------------------------------------------------

void ARBASE_API EnableCategory(int32 category, bool enabled) AR_DEBUG_DRAW_IMPLEMENTATION;
void ARBASE_API ToggleCategory(int32 category) AR_DEBUG_DRAW_IMPLEMENTATION;

// Draw Functions.
void ARBASE_API DrawText(int32 category, NotNullPtr<UWorld> world, const FString& msg,
						 const FColor& color, float lifetime = 0) AR_DEBUG_DRAW_IMPLEMENTATION;

void ARBASE_API DrawBox(int32 category, NotNullPtr<UWorld> world, const FVector& center,
						const FVector& extents, const FColor& color, float thickness,
						float lifetime = 0) AR_DEBUG_DRAW_IMPLEMENTATION;

void ARBASE_API DrawCylinder(int32 category, NotNullPtr<UWorld> world, const FVector& start,
							 const FVector& end, float radius, float segments, const FColor& color,
							 float thickness, float lifetime = 0) AR_DEBUG_DRAW_IMPLEMENTATION;

void ARBASE_API DrawDirectionalArrow(int32 category, NotNullPtr<UWorld> world, const FVector& start,
									 const FVector& end, float arrow_size, const FColor& color,
									 float thickness,
									 float lifetime = 0) AR_DEBUG_DRAW_IMPLEMENTATION;

void ARBASE_API DrawSphere(int32 category, NotNullPtr<UWorld> world, const FVector& center,
						   float radius, float segments, const FColor& color, float thickness,
						   float lifetime = 0) AR_DEBUG_DRAW_IMPLEMENTATION;

// __DebugCategoryRegisterer is a helper class to make a static registration of a debug category.
class ARBASE_API __DebugCategoryRegisterer
{
public:
	explicit __DebugCategoryRegisterer(int32 category, std::string&& name, bool default_enabled,
									   std::string&& description, std::string&& file,
									   int line) AR_DEBUG_DRAW_IMPLEMENTATION;
};

} // namespace debug


#undef AR_DEBUG_DRAW_IMPLEMENTATION
