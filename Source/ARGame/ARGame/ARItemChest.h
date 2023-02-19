﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ARGameplayInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ARItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ARGAME_API AARItemChest : public AActor, public IARGameplayInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AARItemChest();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// BEGIN IARGameplayInterface
	void Interact_Implementation(APawn* Interactor);

	// END IARGameplayInterface

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	float TargetPitch = 110.0f;

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* LidMesh;
};