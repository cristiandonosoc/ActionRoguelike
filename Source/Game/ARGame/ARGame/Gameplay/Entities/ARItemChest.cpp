// Fill out your copyright notice in the Description page of Project Settings.

#include <ARGame/Gameplay/Entities/ARItemChest.h>
#include <Net/UnrealNetwork.h>

// Sets default values
AARItemChest::AARItemChest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if
	// you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	SetReplicates(true);
}

void AARItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& props) const
{
	Super::GetLifetimeReplicatedProps(props);

	// Unreal expects this stupid ass name.
	auto& OutLifetimeProps = props;
	DOREPLIFETIME(AARItemChest, IsOpened);
}

bool AARItemChest::CanInteract_Implementation(APawn* interactor)
{
	return true;
}

bool AARItemChest::Interact_Implementation(APawn* interactor)
{
	IsOpened = !IsOpened;
	ToggleChestAnimation();
	return true;
}

void AARItemChest::OnRep_IsOpenedChanged()
{
	ToggleChestAnimation();
}

void AARItemChest::ToggleChestAnimation()
{
	float target_pitch = IsOpened ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(target_pitch, 0.0f, 0.0f));
}