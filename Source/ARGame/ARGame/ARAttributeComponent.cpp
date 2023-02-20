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
	UE_LOG(LogTemp, Log, TEXT("Delta: %f, Change: %f -> %f"), delta, prev, Health);
	return true;
}


// Called when the game starts
void UARAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}