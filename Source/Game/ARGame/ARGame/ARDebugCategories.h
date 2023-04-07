#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include <ARBase/DebugTools.h>

// We use this macro to ensure each category will get a different number (as long as they are
// each defined in a separate line).
//
// IMPORTANT: This means that ids *WILL NOT* be stable across builds (if the categories are
//            changed). If there is ever a need for that, then another strategy for defining
//            categories ids are needed.
#define UNIQUE_CATEGORY_ID __LINE__

// DECLARE_DEBUG_CATEGORY declares a new category id for our debug system, as well as an UE log
// category.
//
// IMPORTANT: Any category declared here has to have the equivalent AR_DEFINE_DEBUG_CATEGORY call
//            within the .cpp!
#define DECLARE_DEBUG_CATEGORY(ns, category_name, category_variable, ue_default_verbosity,         \
							   ue_compile_time_verbosity)                                          \
	namespace ns                                                                                   \
	{                                                                                              \
	static constexpr uint32 category_variable = UNIQUE_CATEGORY_ID;                                \
	}                                                                                              \
	ARGAME_API DECLARE_LOG_CATEGORY_EXTERN(category_name, ue_default_verbosity,                    \
										   ue_compile_time_verbosity);

// Debug Categories --------------------------------------------------------------------------------
//
// IMPORTANT: Any category declared here has to have the equivalent AR_DEFINE_DEBUG_CATEGORY call
//            within the .cpp!

DECLARE_DEBUG_CATEGORY(ar, LogAR_Always, ALWAYS, Log, All);
DECLARE_DEBUG_CATEGORY(ar, LogAR_Actions, ACTIONS, Log, All);
DECLARE_DEBUG_CATEGORY(ar, LogAR_Attributes, ATTRIBUTES, Log, All);
DECLARE_DEBUG_CATEGORY(ar, LogAR_AI, AI, Log, All);
DECLARE_DEBUG_CATEGORY(ar, LogAR_Entities, ENTITIES, Log, All);
DECLARE_DEBUG_CATEGORY(ar, LogAR_GameMode, GAME_MODE, Log, All);
DECLARE_DEBUG_CATEGORY(ar, LogAR_Interaction, INTERACTION, Log, All);
DECLARE_DEBUG_CATEGORY(ar, LogAR_PlayerCharacter, PLAYER_CHARACTER, Log, All);
DECLARE_DEBUG_CATEGORY(ar, LogAR_Projectiles, PROJECTILES, Log, All);
DECLARE_DEBUG_CATEGORY(ar, LogAR_UI, UI, Log, All);

#undef DECLARE_DEBUG_CATEGORY
#undef UNIQUE_CATEGORY_ID