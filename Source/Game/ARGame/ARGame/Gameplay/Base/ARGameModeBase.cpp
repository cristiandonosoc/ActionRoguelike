#include <ARGame/Gameplay/Base/ARGameModeBase.h>

#include <ARBase/Logging.h>
#include <ARBase/NotNullPtr.h>
#include <ARBase/Subsystems/ARStreamingSubsystem.h>
#include <ARGame/AI/ARAICharacter.h>
#include <ARGame/ARDebugCategories.h>
#include <ARGame/Gameplay/ARCharacter.h>
#include <ARGame/Gameplay/Components/ARAttributeComponent.h>

#include <EngineUtils.h>
#include <EnvironmentQuery/EnvQueryManager.h>

namespace
{

TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("ar.GameMode.SpawnBots"), true,
												TEXT("Enable spawning of bots via a timer"),
												ECVF_Cheat);
												
												
} // namespace

void AARGameModeBase::StartPlay()
{
	Super::StartPlay();

	// TODO(cdc): Do async loading.
	NotNullPtr streamer = GetGameInstance()->GetSubsystem<UARStreamingSubsystem>();
	streamer->RequestSyncLoad(BotClassToSpawn);

	// On start, we start a timer to spawn a new bot.
	if (MaxConcurrentBots > 0)
	{
		GetWorldTimerManager().SetTimer(SpawnBotTimerHandle, this,
										&AARGameModeBase::OnSpawnBotTimerElapsed, SpawnBotInterval,
										true);
	}
}

namespace
{

void SchedulePlayerForRevive(NotNullPtr<AARGameModeBase> game_mode, NotNullPtr<AARCharacter> player)
{
	// Create a respawn delay.
	FTimerHandle respawn_handle;
	FTimerDelegate delegate;
	delegate.BindUFunction(game_mode.Get(), "RespawnPlayerTimerElapsed", player->GetController());
	game_mode->GetWorldTimerManager().SetTimer(respawn_handle, std::move(delegate),
											   game_mode->GetReviveInterval(), false);
}

void PayCreditsToKiller(NotNullPtr<AActor> killer, uint32 credits)
{
	if (credits <= 0)
	{
		return;
	}

	if (!killer->Implements<UARCreditHolder>())
	{
		return;
	}

	IARCreditHolder::Execute_AddCredits(killer, credits);
}

} // namespace

void AARGameModeBase::OnActorKilled(NotNullPtr<AActor> victim, AActor* killer, uint32 credits)
{
	AR_LOG_CSS(GetWorld(), LogAR_GameMode, Log, TEXT("Actor %s died, killed by %s"),
			   *GetNameSafe(victim), *GetNameSafe(killer));

	// Check if it's a player.
	if (AARCharacter* player = Cast<AARCharacter>(victim.Get()))
	{
		SchedulePlayerForRevive(this, player);
	}

	if (!killer)
	{
		return;
	}
	PayCreditsToKiller(killer, credits);
}

void AARGameModeBase::RespawnPlayerTimerElapsed(AController* player_controller)
{
	if (ensure(player_controller))
	{
		player_controller->UnPossess();
		RestartPlayer(player_controller);
	}
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
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		AR_LOG_CSS(GetWorld(), LogAR_GameMode, Error, TEXT("GameMode: CVarSpawnBots not set"));
		return;
	}

	if (!SpawnLocationEnvQuery)
	{
		AR_LOG_CSS(GetWorld(), LogAR_GameMode, Error,
				   TEXT("GameMode: SpawnLocationEnvQuery not set"));
		return;
	}

	if (!BotClassToSpawn)
	{
		AR_LOG_CSS(GetWorld(), LogAR_GameMode, Error, TEXT("GameMode: BotClassToSpawn not set"));
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

	check(max_bots > 0);

	AR_LOG_CSS(GetWorld(), LogAR_GameMode, Log,
			   TEXT("GameMode: Spawn timer elapsed! Current bots: %d/%d"), alive_bots, max_bots);
	if (alive_bots >= max_bots)
	{
		return;
	}

	// Perform an EQS request.
	AR_LOG_CSS(GetWorld(), LogAR_GameMode, Log,
			   TEXT("GameMode: Attempting to spawn bot: Running EQS query"));

	FEnvQueryRequest env_request(SpawnLocationEnvQuery, this);
	int32 query_id = env_request.Execute(EEnvQueryRunMode::RandomBest25Pct, this,
										 &AARGameModeBase::OnSpawnLocationEnvQueryResult);
	check(query_id != INDEX_NONE);
}

void AARGameModeBase::OnSpawnLocationEnvQueryResult(TSharedPtr<FEnvQueryResult> result)
{
	if (!result->IsFinished())
	{
		AR_LOG_CSS(GetWorld(), LogAR_GameMode, Error,
				   TEXT("GameMode: SpawnLocationEnvQuery not finished"));
		return;
	}

	if (!result->IsSuccessful())
	{
		AR_LOG_CSS(GetWorld(), LogAR_GameMode, Error,
				   TEXT("GameMode: SpawnLocationEnvQuery not successful"));
		return;
	}

	AR_LOG_CSS(GetWorld(), LogAR_GameMode, Log,
			   TEXT("GameMode: EQS query successful! Spawning bot"));

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
		AR_LOG_CSS(GetWorld(), LogAR_GameMode, Error, TEXT("GameMode: spawning class %s"),
				   *BotClassToSpawn->GetName());
		return;
	}

	// Draw a vertical cylinder where the bot was spawned.
	constexpr float HEIGHT = 1000.0f;
	FVector start = target_location - FVector(0, 0, -HEIGHT);
	FVector end = target_location - FVector(0, 0, HEIGHT);
	DrawDebugCylinder(GetWorld(), start, end, 50.0f, 32, FColor::Blue, false, 10.0f);
}