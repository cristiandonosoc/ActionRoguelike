#include <ARGameServer/Gameplay/Components/AttributeComponentServer.h>

#include <ARGame/ARDebugCategories.h>
#include <ARGame/Gameplay/Base/ARGameModeBase.h>
#include <ARGame/Gameplay/Components/ARAttributeComponent.h>

namespace ar
{
namespace server
{

namespace
{

TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("ar.Attributes.DamageMultiplier"), 1.0f,
												 TEXT("How much damage should be multiplied on"),
												 ECVF_Cheat);

} // namespace


bool AttributeComponentServer::ApplyHealthChange(NotNullPtr<AActor> instigator, float delta)
{
	if (delta < 0.0f)
	{
		delta *= CVarDamageMultiplier.GetValueOnGameThread();
	}

	if (!GetBase()->WouldHealthChangeApply(delta))
	{
		return false;
	}

	// Now that the health change has been validated, we can apply it.
	// TODO(cdc): Communicate as well the actual delta if it was clamped.
	float prev = GetBase()->GetHealth();
	float expectedResult = prev + delta;

	float new_health = FMath::Clamp(expectedResult, 0, GetBase()->GetMaxHealth());
	GetBase()->SetHealth(new_health);

	UE_LOG(LogAR_Attributes, Log, TEXT("Delta: %f, Change: %f -> %f"), delta, prev, new_health);

	// Trigger the delegate.
	FOnHealthChangedPayload payload = {};
	payload.Instigator = instigator;
	payload.Target = GetBase();
	payload.MaxHealth = GetBase()->GetMaxHealth();
	payload.NewHealth = new_health;
	payload.OriginalDelta = delta;
	payload.ActualDelta = new_health - prev;

	// Check the flags.
	if (new_health == 0.0f && prev > 0.0f)
	{
		payload.SetKilled();

		// Let the game mode know this character was killed.
		// TODO(cdc): This should be via an event.
		NotNullPtr game_mode = GetBase()->GetWorld()->GetAuthGameMode<AARGameModeBase>();
		game_mode->OnActorKilled(GetBase()->GetOwner(), instigator, GetBase()->GetKilledCredits());
	}

	GetBase()->OnHealthChanged.Broadcast(payload);

	return true;
}

} // namespace server
} // namespace ar