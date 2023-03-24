// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <ARGame/Gameplay/Projectiles/ARBaseProjectile.h>

#include <CoreMinimal.h>
#include <GameplayTagContainer.h>

#include "ARMagicProjectile.generated.h"

class UARActionEffect;
class UParticleSystem;
class USoundCue;

UCLASS()
class ARGAME_API AARMagicProjectile : public AARBaseProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AARMagicProjectile();

	// Called every frame
	virtual void Tick(float delta) override;

	virtual void OnBeginHit_Implementation(UPrimitiveComponent* hit_component, AActor* other_actor,
										   UPrimitiveComponent* other_comp, FVector normal_impulse,
										   const FHitResult& hit) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Attacks")
	float Damage = 80;

	UPROPERTY(EditDefaultsOnly, Category = "Attacks")
	FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly, Category = "Attacks")
	TSubclassOf<UARActionEffect> ActionEffect;

	UPROPERTY(EditAnywhere, Category = "Particles")
	TObjectPtr<UParticleSystem> ExplosionParticle;

	UPROPERTY(EditAnywhere, Category = "Audio")
	TObjectPtr<USoundCue> TravelSound;

	UPROPERTY(EditAnywhere, Category = "Audio")
	TObjectPtr<USoundCue> ExplosionSound;
};
