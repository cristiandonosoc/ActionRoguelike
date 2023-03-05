// Fill out your copyright notice in the Description page of Project Settings.


#include "ARHealthPack.h"


#include "ARBase/NotNullPtr.h"
#include "ARGame/ARAttributeComponent.h"


// Sets default values
AARHealthPack::AARHealthPack()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if
	// you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AARHealthPack::BeginPlay()
{
	Super::BeginPlay();
}

bool AARHealthPack::CanUse_Implementation(APawn* interactor)
{
	Super::CanUse_Implementation(interactor);

	// Check if the user has some health attributes.
	auto* attributes = Cast<UARAttributeComponent>(
		interactor->GetComponentByClass(UARAttributeComponent::StaticClass()));
	if (!attributes)
	{
		return false;
	}

	// If it would not change the player's health, we don't do anything.
	if (!attributes->WouldHealthChangeApply(HealAmount))
	{
		return false;
	}

	return true;
}

void AARHealthPack::Use_Implementation(APawn* interactor)
{
	Super::Use_Implementation(interactor);

	// At this point, it should be validated by CanUse that this interactor can interact with
	// this pawn.
	NotNullPtr attributes = Cast<UARAttributeComponent>(
		interactor->GetComponentByClass(UARAttributeComponent::StaticClass()));

	bool affected = attributes->ApplyHealthChange(this, HealAmount);
	check(affected);
}