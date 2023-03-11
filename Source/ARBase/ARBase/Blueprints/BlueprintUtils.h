#pragma once

#include <CoreMinimal.h>

#include "BlueprintUtils.generated.h"

UCLASS()
class UBlueprintUtils final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// GetAccurateTime returns the time since the editor has started.
	// This is useful for UI, in which the time nodes do not restart when the game starts, but
	// rather remain to run in the editor it seems.
	//
	// More info:
	// https://forums.unrealengine.com/t/the-time-material-node-returns-editor-up-time-instead-of-game-time-if-the-material-is-set-to-the-material-domain-user-interface/398968/11
	UFUNCTION(BlueprintPure, Category = "Time")
	static float GetAccurateTime();
};
