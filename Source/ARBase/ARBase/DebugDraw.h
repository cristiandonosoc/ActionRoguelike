#pragma once

#include <ARBase/BuildDefines.h>
#include <ARBase/Macros.h>
#include <ARBase/NotNullPtr.h>

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

// AR_REGISTER_DEBUG_CATEGORY registers a new category so that code can enable/disable the category
// on runtime in a convenient manner.
//
// This way code can just call always debug draw from the code and they will only run if the
// category is enabled.
//
// All of this code is automatically stripped out in release builds.
#define AR_REGISTER_DEBUG_CATEGORY(category, default_enabled, description)                         \
	static ::__DebugCategoryRegisterer AR_CONCAT(__debug_draw_category_registerer__, __LINE__)(    \
		category, #category, default_enabled, description, __FILE__, __LINE__)

// ARDebugDraw represents all the debug draw capabilities that are automatically stripped out in
// release builds, so gameplay code can always call.
class ARBASE_API ARDebugDraw
{
public:
#if AR_DEBUG_DRAW_ENABLED
	static bool IsCategoryEnabled(int32 category);
#else
	FORCEINLINE static bool IsCategoryEnabled(int32 category) { return false; }
#endif // AR_DEBUG_DRAW_ENABLED


	static void EnableCategory(int32 category, bool enabled) AR_DEBUG_DRAW_IMPLEMENTATION;
	static void ToggleCategory(int32 category) AR_DEBUG_DRAW_IMPLEMENTATION;

	// Draw Functions.

	static void Cylinder(int32 category, NotNullPtr<UWorld> world, const FVector& start,
						 const FVector& end, float radius, float segments, const FColor& color,
						 float lifetime, float thickness) AR_DEBUG_DRAW_IMPLEMENTATION;

	static void DirectionalArrow(int32 category, NotNullPtr<UWorld> world, const FVector& start,
								 const FVector& end, float arrow_size, const FColor& color,
								 float lifetime, float thickness);

	static void Sphere(int32 category, NotNullPtr<UWorld> world, const FVector& center,
					   float radius, float segments, const FColor& color, float lifetime,
					   float thickness) AR_DEBUG_DRAW_IMPLEMENTATION;
};

class ARBASE_API __DebugCategoryRegisterer
{
public:
	explicit __DebugCategoryRegisterer(int32 category, std::string&& name, bool default_enabled,
									   std::string&& description, std::string&& file,
									   int line) AR_DEBUG_DRAW_IMPLEMENTATION;
};

#undef AR_DEBUG_DRAW_IMPLEMENTATION