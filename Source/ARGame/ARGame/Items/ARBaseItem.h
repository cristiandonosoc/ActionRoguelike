// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ARBase/NotNullPtr.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ARBaseItem.generated.h"

UCLASS()
class ARGAME_API AARBaseItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AARBaseItem();

	// Called every frame
	virtual void Tick(float delta) override;

	// TriggerUse is meant to be called with a pawn to trigger the use call to the object, which
	// will implement the event either in C++ or in blueprint.
	// This is mostly to be called by ItemSpawner.
	void TriggerUse(NotNullPtr<APawn> interactor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void Use(APawn* interactor);
	
	virtual void Use_Implementation(APawn* interactor) {}
};