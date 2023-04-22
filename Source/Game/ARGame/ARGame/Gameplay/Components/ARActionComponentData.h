#pragma once

#include "ARActionComponentData.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FPredictedStartActionContext
{
	GENERATED_BODY()

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FRotator Rotation;
};