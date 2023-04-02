// Fill out your copyright notice in the Description page of Project Settings.

#include <ARGame/Gameplay/Components/ARAttributeComponent.h>
#include <ARGame/Gameplay/Base/ARGameModeBase.h>
#include <Particles/Collision/ParticleModuleCollisionGPU.h>


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

bool UARAttributeComponent::Server_ApplyHealthChange(AActor* instigator, float delta)
{
	CHECK_RUNNING_ON_SERVER();
	return GetServerSplit()->ApplyHealthChange(instigator, delta);
}