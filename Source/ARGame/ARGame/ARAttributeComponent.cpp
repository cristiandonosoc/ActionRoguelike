// Fill out your copyright notice in the Description page of Project Settings.


#include "ARAttributeComponent.h"

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

bool UARAttributeComponent::ApplyHealthChange(float delta)
{
	if (!WouldHealthChangeApply(delta))
	{
		return false;
	}

	// Now that the health change has been validated, we can apply it.
	// TODO(cdc): Communicate as well the actual delta if it was clamped.
	float prev = Health;
	Health += delta;
	Health = FMath::Clamp(Health, 0, MaxHealth);

	UE_LOG(LogTemp, Log, TEXT("Delta: %f, Change: %f -> %f"), delta, prev, Health);

	// Trigger the delegate.
	FOnHealthChangedPayload payload = {};
	payload.Target = this;
	payload.MaxHealth = MaxHealth;
	payload.NewHealth = Health;
	payload.Delta = delta;
	OnHealthChanged.Broadcast(payload);

	return true;
}
bool UARAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}


// Called when the game starts
void UARAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}