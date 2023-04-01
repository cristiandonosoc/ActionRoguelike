﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <ARGame/Items/ARBaseItem.h>

#include <CoreMinimal.h>

#include "ARHealthPack.generated.h"

UCLASS()
class ARGAME_API AARHealthPack : public AARBaseItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AARHealthPack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// INTERFACE_BEGIN(ARBaseItem)
	virtual bool CanUse_Implementation(APawn* interactor) override;
	virtual void Use_Implementation(APawn* interactor) override;
	// INTERFACE_END(ARBaseItem)

protected:
	UPROPERTY(EditDefaultsOnly)
	float HealAmount;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;
};