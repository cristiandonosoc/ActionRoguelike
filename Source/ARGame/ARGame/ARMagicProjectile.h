// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARBaseProjectile.h"

#include "ARMagicProjectile.generated.h"

UCLASS()
class ARGAME_API AARMagicProjectile : public AARBaseProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AARMagicProjectile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};