// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "ARInteractionComponent.generated.h"

// UARInteractionComponent handles the interactivity part of the character.
//
// Currently it is not meant to be ticked directly, but rather it is meant to be called
// directly from the character. This is because the character holds much more information
// about the world, camera and where the player is looking at.
UCLASS()
class ARGAME_API UARInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UARInteractionComponent();

	// target is where the camera is targeting.
	void PrimaryInteract(const FVector& camera_target);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
