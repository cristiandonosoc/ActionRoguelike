// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <ARBase/NotNullPtr.h>
#include <Components/ActorComponent.h>
#include <CoreMinimal.h>

#include "ARInteractionComponent.generated.h"

class IARInteractable;
class UARActorAttachedWidget;

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
	static constexpr float kInteractionDistance = 1000.0f;
	static constexpr float kInteractionRadius = 30.0f;

	static constexpr float kFocusCheckPeriod = 0.25f;

	// Sets default values for this component's properties
	UARInteractionComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float delta, ELevelTick tick_type,
							   FActorComponentTickFunction* tick_function) override;


	// Interact against the current best interactable.
	void PrimaryInteract();

protected:
	UFUNCTION()
	void FindBestInteractable();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UARActorAttachedWidget> DefaultWidgetClass;

	UPROPERTY()
	TObjectPtr<UARActorAttachedWidget> Widget;

private:
	FTimerHandle FindFocusTimerHandle;
	TWeakObjectPtr<AActor> FocusedInteractable;
};