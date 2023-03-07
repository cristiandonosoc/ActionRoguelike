// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

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

	UFUNCTION(BlueprintNativeEvent)
	bool CanUse(APawn* interactor);

	UFUNCTION(BlueprintNativeEvent)
	void Use(APawn* interactor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual bool CanUse_Implementation(APawn* interactor);

	// Use attempts to use the item on the given interactor.
	// Use CanUse to check whether the item would be used on the given interactor.
	virtual void Use_Implementation(APawn* interactor);
};