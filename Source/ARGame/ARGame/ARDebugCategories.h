﻿#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include <ARBase/DebugDraw.h>

// ARDebugCategories tracks all the categories to be used in this module.
// Make sure these categories don't collide.
class ARDebugCategories
{
public:
	static constexpr uint32 INTERACTION = 0;
	static constexpr uint32 PLAYER_CHARACTER = 1;
};