// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

#include "ARTargetDummy.generated.h"

struct FOnHealthChangedPayload;
class UARAttributeComponent;

class UStaticMeshComponent;

UCLASS()
class ARGAME_API AARTargetDummy : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AARTargetDummy();

	// Called every frame
	virtual void Tick(float delta) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHealthChanged(const FOnHealthChangedPayload& payload);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UARAttributeComponent> Attributes;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
};
