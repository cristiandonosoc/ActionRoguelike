// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/GameModeBase.h>

#include "ARGameModeBase.generated.h"

class AARAICharacter;

struct FEnvQueryResult;
class UEnvQuery;
class UCurveFloat;

UCLASS()
class ARGAME_API AARGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AARGameModeBase();

	virtual void StartPlay() override;

private:
	void OnSpawnBotTimerElapsed();
	void OnSpawnLocationEnvQueryResult(TSharedPtr<FEnvQueryResult> result);

protected:
	// Time in seconds to spawn a new AI bot.
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnBotInterval = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	int32 MaxConcurrentBots = 4;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UCurveFloat> SpawnCurve;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UEnvQuery> SpawnLocationEnvQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSoftClassPtr<AARAICharacter> BotClassToSpawn;


private:
	FTimerHandle SpawnBotTimerHandle;
};