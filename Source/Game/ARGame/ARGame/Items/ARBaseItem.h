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

	// INTERFACE_BEGIN(AARBaseItem)
	virtual bool CanUse_Implementation(APawn* interactor);
	virtual void Use_Implementation(APawn* interactor);
	// INTERFACE_END(AARBaseItem)

protected:
	// How many credits are needed to use this item.
	UPROPERTY(EditDefaultsOnly)
	int32 Price = 0;
};