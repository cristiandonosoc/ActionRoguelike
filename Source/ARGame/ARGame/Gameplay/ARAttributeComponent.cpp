// Fill out your copyright notice in the Description page of Project Settings.

#include <ARGame/Gameplay/ARAttributeComponent.h>
#include <ARGame/Gameplay/Base/ARGameModeBase.h>
#include <Particles/Collision/ParticleModuleCollisionGPU.h>

static TAutoConsoleVariable<float>
	CVarDamageMultiplier(TEXT("ar.Attributes.DamageMultiplier"), 1.0f,
						 TEXT("How much damage should be multiplied on"), ECVF_Cheat);

bool UARAttributeComponent::IsActorAlive(NotNullPtr<AActor> actor)
{
	auto& attributes = GetAttributes(actor);
	return attributes.IsAlive();
}

UARAttributeComponent& UARAttributeComponent::GetAttributes(NotNullPtr<AActor> actor)
{
	auto* comp = actor->FindComponentByClass<UARAttributeComponent>();
	check(comp);
	return *comp;
}

// Sets default values for this component's properties
UARAttributeComponent::UARAttributeComponent() {}

bool UARAttributeComponent::WouldHealthChangeApply(float delta) const
{
	if (delta == 0.0f)
	{
		return false;
	}

	// If they're healing, we see if it's needed.
	if (delta > 0.0f)
	{
		if (Health >= MaxHealth)
		{
			return false;
		}
		return true;
	}

	// Check if damage can be done.
	if (Health < 0.0f)
	{
		return false;
	}

	return true;
}

bool UARAttributeComponent::ApplyHealthChange(AActor* instigator, float delta)
{
	if (delta < 0.0f)
	{
		delta *= CVarDamageMultiplier.GetValueOnGameThread();
	}

	if (!WouldHealthChangeApply(delta))
	{
		return false;
	}

	// Now that the health change has been validated, we can apply it.
	// TODO(cdc): Communicate as well the actual delta if it was clamped.
	float prev = Health;
	float expectedResult = prev + delta;
	Health = FMath::Clamp(expectedResult, 0, MaxHealth);

	UE_LOG(LogTemp, Log, TEXT("Delta: %f, Change: %f -> %f"), delta, prev, Health);

	// Trigger the delegate.
	FOnHealthChangedPayload payload = {};
	payload.Instigator = instigator;
	payload.Target = this;
	payload.MaxHealth = MaxHealth;
	payload.NewHealth = Health;
	payload.OriginalDelta = delta;
	payload.ActualDelta = Health - prev;

	// Check the flags.
	if (Health == 0.0f && prev > 0.0f)
	{
		payload.SetKilled();

		// Let the game mode know this character was killed.
		NotNullPtr game_mode = GetWorld()->GetAuthGameMode<AARGameModeBase>();
		game_mode->OnActorKilled(GetOwner(), instigator, KilledCredits);
	}

	OnHealthChanged.Broadcast(payload);

	return true;
}