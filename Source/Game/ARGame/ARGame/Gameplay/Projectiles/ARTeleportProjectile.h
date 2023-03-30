// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <ARGame/Gameplay/Projectiles/ARBaseProjectile.h>
#include <CoreMinimal.h>

#include "ARTeleportProjectile.generated.h"

class UParticleSystemComponent;
class UPrimitiveComponent;

UCLASS()
class ARGAME_API AARTeleportProjectile : public AARBaseProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AARTeleportProjectile();

	// Called every frame
	virtual void Tick(float delta) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	virtual void OnBeginHit_Implementation(UPrimitiveComponent* hit_component, AActor* other_actor,
										   UPrimitiveComponent* other_comp, FVector normal_impulse,
										   const FHitResult& hit) override;

private:
	void ExplosionTimerStart();
	void ExplosionTimerEnd();

	void TeleportInstigator();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UParticleSystemComponent> ExplosionEffect;

	// Timer for the max time the projectile can be flying.
	float FlyingTimerDelay = 0.2;
	FTimerHandle FlyingTimerHandle;

	// Timer for the time an explosion effect will play after colliding/expiring timer.
	// After that timer, the player will teleport.
	float ExplosionTimerDelay = 0.2f;
	FTimerHandle ExplosionTimerHandle;
};