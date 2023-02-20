// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "ARAttributeComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ARGAME_API UARAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UARAttributeComponent();

	// Returns whether the change was applied.
	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool ApplyHealthChange(float delta);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float Health = 100;

	// HealthMax, Stamina, Strength
		
};
