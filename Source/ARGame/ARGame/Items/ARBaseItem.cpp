// Fill out your copyright notice in the Description page of Project Settings.


#include "ARBaseItem.h"


// Sets default values
AARBaseItem::AARBaseItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if
	// you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AARBaseItem::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AARBaseItem::Tick(float delta)
{
	Super::Tick(delta);
}

bool AARBaseItem::CanUse_Implementation(APawn* interactor)
{
	check(interactor);
	return true;
}

void AARBaseItem::Use_Implementation(APawn* interactor)
{
	check(CanUse(interactor));
	check(interactor);
}