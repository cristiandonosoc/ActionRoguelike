#include <ARGame/Gameplay/Components/ARAttributeComponent.h>

#include <ARGame/Gameplay/Base/ARGameModeBase.h>
#include <Net/UnrealNetwork.h>
#include <Particles/Collision/ParticleModuleCollisionGPU.h>

UARAttributeComponent::UARAttributeComponent()
{
	INIT_BASE_CLIENT_SERVER_SPLIT();
}

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

void UARAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& props) const
{
	Super::GetLifetimeReplicatedProps(props);

	// Unreal expects this stupid ass name.
	auto& OutLifetimeProps = props;
	DOREPLIFETIME(UARAttributeComponent, Health);
	DOREPLIFETIME(UARAttributeComponent, MaxHealth);
}

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
	CHECK_RUNNING_ON_SERVER(this);
	return GetServerSplit()->ApplyHealthChange(instigator, delta);
}