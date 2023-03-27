#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include <ARBase/DebugDraw.h>

// We use this macro to ensure each category will get a different number (as long as they are
// each defined in a separate line).
//
// IMPORTANT: This means that ids *WILL NOT* be stable across builds (if the categories are
//            changed). If there is ever a need for that, then another strategy for defining
//            categories ids are needed.
#define UNIQUE_CATEGORY_ID __LINE__

// ARDebugCategories tracks all the categories to be used in this module.
// Make sure these categories don't collide.
class ARDebugCategories
{
public:
	// ALWAYS is special in that it will always work if there is debug support enable.
	// This still won't work on release builds.
	static constexpr uint32 ALWAYS = UNIQUE_CATEGORY_ID;
	static constexpr uint32 ACTIONS = UNIQUE_CATEGORY_ID;
	static constexpr uint32 INTERACTION = UNIQUE_CATEGORY_ID;
	static constexpr uint32 PLAYER_CHARACTER = UNIQUE_CATEGORY_ID;
	static constexpr uint32 PROJECTILES = UNIQUE_CATEGORY_ID;
};

#undef UNIQUE_CATEGORY_ID