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


// Debug Categories --------------------------------------------------------------------------------
//
// IMPORTANT: Any category declared here has to have the equivalent AR_DEFINE_DEBUG_CATEGORY call
//            within the .cpp!

DECLARE_DEBUG_CATEGORY(ar, ARGAME_API, LogAR_Always, ALWAYS, Log, All);
DECLARE_DEBUG_CATEGORY(ar, ARGAME_API, LogAR_Actions, ACTIONS, Log, All);
DECLARE_DEBUG_CATEGORY(ar, ARGAME_API, LogAR_Attributes, ATTRIBUTES, Log, All);
DECLARE_DEBUG_CATEGORY(ar, ARGAME_API, LogAR_AI, AI, Log, All);
DECLARE_DEBUG_CATEGORY(ar, ARGAME_API, LogAR_Entities, ENTITIES, Log, All);
DECLARE_DEBUG_CATEGORY(ar, ARGAME_API, LogAR_GameMode, GAME_MODE, Log, All);
DECLARE_DEBUG_CATEGORY(ar, ARGAME_API, LogAR_Interaction, INTERACTION, Log, All);
DECLARE_DEBUG_CATEGORY(ar, ARGAME_API, LogAR_PlayerCharacter, PLAYER_CHARACTER, Log, All);
DECLARE_DEBUG_CATEGORY(ar, ARGAME_API, LogAR_Projectiles, PROJECTILES, Log, All);
DECLARE_DEBUG_CATEGORY(ar, ARGAME_API, LogAR_UI, UI, Log, All);

#undef DECLARE_DEBUG_CATEGORY
#undef UNIQUE_CATEGORY_ID
