// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ARBaseProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS()
class ARGAME_API AARBaseProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AARBaseProjectile();

	// Called every frame
	virtual void Tick(float delta) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* Effect;
};