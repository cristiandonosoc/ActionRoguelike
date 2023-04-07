#include <ARGame/ARDebugCategories.h>

AR_DEFINE_DEBUG_CATEGORY(ar::ALWAYS, LogAR_Always, Always, true,
						 "General categories that should be always on.");

AR_DEFINE_DEBUG_CATEGORY(ar::AI, LogAR_AI, AI, true, "All about AI");

AR_DEFINE_DEBUG_CATEGORY(ar::ACTIONS, LogAR_Actions, Actions, true, "All about actions");

AR_DEFINE_DEBUG_CATEGORY(ar::ATTRIBUTES, LogAR_Attributes, Attributes, true,
						 "All about entities attributes");

AR_DEFINE_DEBUG_CATEGORY(ar::ENTITIES, LogAR_Entities, Entities, true,
						 "All the displays for player interactions");

AR_DEFINE_DEBUG_CATEGORY(ar::GAME_MODE, LogAR_GameMode, GameMode, true,
						 "All the displays for game mode");

AR_DEFINE_DEBUG_CATEGORY(ar::INTERACTION, LogAR_Interaction, Interaction, true,
						 "All the displays for player interactions");

AR_DEFINE_DEBUG_CATEGORY(ar::PLAYER_CHARACTER, LogAR_PlayerCharacter, PlayerCharacter, false,
						 "All the display about the player character");

AR_DEFINE_DEBUG_CATEGORY(ar::PROJECTILES, LogAR_Projectiles, Projectiles, true,
						 "All the display about projectiles");

AR_DEFINE_DEBUG_CATEGORY(ar::UI, LogAR_UI, UI, true, "All the display about projectiles");