// Fill out your copyright notice in the Description page of Project Settings.


#include "ARAttributeComponent.h"

// Sets default values for this component's properties
UARAttributeComponent::UARAttributeComponent()
{
}

bool UARAttributeComponent::ApplyHealthChange(float delta)
{
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
