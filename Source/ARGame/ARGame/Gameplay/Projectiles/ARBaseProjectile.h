// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

#include "ARBaseProjectile.generated.h"

class UAudioComponent;
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
	virtual void PostInitializeComponents() override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// OnBeginHitInternal is the way we propagate the overlap result to derived classes.
	// It does some common checks before triggering the actual event |OnBeginHit|.
	// This is called by the collision sphere and then propagated as results to the projectiles.
	UFUNCTION()
	void OnBeginHitInternal(UPrimitiveComponent* hit_component, AActor* other_actor,
							UPrimitiveComponent* other_comp, FVector normal_impulse,
							const FHitResult& hit);

	// OnBeginHit is the wrapper expose function that we hook to the collision/blueprint system.
	// Derived projectiles are expected to override |OnBeginHitDerived|.
	UFUNCTION(BlueprintNativeEvent)
	void OnBeginHit(UPrimitiveComponent* hit_component, AActor* other_actor,
					UPrimitiveComponent* other_comp, FVector normal_impulse, const FHitResult& hit);


	virtual void OnBeginHit_Implementation(UPrimitiveComponent* hit_component, AActor* other_actor,
										   UPrimitiveComponent* other_comp, FVector normal_impulse,
										   const FHitResult& hit) 
	{
		// NOTE: Meant to be derived if needed.
	}


protected:
	UPROPERTY(EditAnywhere, Category = "Projectile")
	float ProjectileSpeed = 1000.0f;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	UParticleSystemComponent* Effect;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<UParticleSystem> MuzzleEffect;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAudioComponent> Audio;
};
