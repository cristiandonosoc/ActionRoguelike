// Copyright Epic Games, Inc. All Rights Reserved.

#include <ARGame/ARGameModeBase.h>

#include <ARGame/AI/ARAICharacter.h>
#include <ARGame/ARAttributeComponent.h>
#include <ARBase/NotNullPtr.h>
#include <ARBase/Subsystems/ARStreamingSubsystem.h>

#include <EngineUtils.h>
#include <EnvironmentQuery/EnvQueryManager.h>

AARGameModeBase::AARGameModeBase() {}

void AARGameModeBase::StartPlay()
{
	Super::StartPlay();
	
	// TODO(cdc): Do async loading.
	NotNullPtr streamer = GetGameInstance()->GetSubsystem<UARStreamingSubsystem>();
	streamer->RequestSyncLoad(BotClassToSpawn);

	// On start, we start a timer to spawn a new bot.
	GetWorldTimerManager().SetTimer(SpawnBotTimerHandle, this,
									&AARGameModeBase::OnSpawnBotTimerElapsed, SpawnBotInterval,
									true);
}

namespace
{

int32 CountAliveBots(NotNullPtr<UWorld> world)
{
	// Determine how many bots currently are in the game.
	int32 result = 0;
	for (TActorIterator<AARAICharacter> it(world); it; ++it)
	{
		if (UARAttributeComponent::IsActorAlive(*it))
		{
			result++;
		}
	}

	return result;
}

} // namespace

void AARGameModeBase::OnSpawnBotTimerElapsed()
{
	if (!SpawnLocationEnvQuery)
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode: SpawnLocationEnvQuery not set"));
		return;
	}

	if (!BotClassToSpawn)
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode: BotClassToSpawn not set"));
		return;
	}

	// We count the current alive bots and see if we need to spawn more of them.
	int32 alive_bots = CountAliveBots(GetWorld());

	// If there is no SpawnCurve, we limit against a hard max.
	int32 max_bots = MaxConcurrentBots;
	if (SpawnCurve)
	{
		max_bots = static_cast<int32>(SpawnCurve->GetFloatValue(GetWorld()->TimeSeconds));
	}

	UE_LOG(LogTemp, Log, TEXT("GameMode: Spawn timer elapsed! Current bots: %d/%d"), alive_bots,
		   max_bots);
	if (alive_bots >= max_bots)
	{
		return;
	}

	// Perform an EQS request.
	UE_LOG(LogTemp, Log, TEXT("GameMode: Attempting to spawn bot: Running EQS query"));

	FEnvQueryRequest env_request(SpawnLocationEnvQuery, this);
	int32 query_id = env_request.Execute(EEnvQueryRunMode::RandomBest25Pct, this,
										 &AARGameModeBase::OnSpawnLocationEnvQueryResult);
	check(query_id != INDEX_NONE);
}

void AARGameModeBase::OnSpawnLocationEnvQueryResult(TSharedPtr<FEnvQueryResult> result)
{
	if (!result->IsFinished())
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode: SpawnLocationEnvQuery not finished"));
		return;
	}

	if (!result->IsSuccessful())
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode: SpawnLocationEnvQuery not successful"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("GameMode: EQS query successful! Spawning bot"));

	FVector target_location = result->GetItemAsLocation(0);
	check(!target_location.IsNearlyZero());

	// Spawn the bot.
	FTransform spawn_transform = FTransform(FRotator::ZeroRotator, target_location);
	FActorSpawnParameters spawn_params = {};
	spawn_params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	auto* ai_character = GetWorld()->SpawnActor<AARAICharacter>(
		BotClassToSpawn.Get(), std::move(spawn_transform), std::move(spawn_params));
	if (!ai_character)
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode: spawning class %s"), *BotClassToSpawn->GetName());
		return;
	}

	// Draw a vertical cylinder where the bot was spawned.
	constexpr float HEIGHT = 1000.0f;
	FVector start = target_location - FVector(0, 0, -HEIGHT);
	FVector end = target_location - FVector(0, 0, HEIGHT);
	DrawDebugCylinder(GetWorld(), start, end, 50.0f, 32, FColor::Blue, false, 10.0f);
}