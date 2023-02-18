// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ARExplosiveBarrel.generated.h"

class UMeshComponent;
class URadialForceComponent;

UCLASS()
class ACTIONROGUELIKE_API AARExplosiveBarrel : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AARExplosiveBarrel();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

protected:
	UFUNCTION(BlueprintCallable)
	void Explode();
	
	UFUNCTION()
	void OnActorHit(UPrimitiveComponent *HitComponent, AActor *OtherActor,
					UPrimitiveComponent *OtherComponent, FVector Normal, const FHitResult &Hit);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URadialForceComponent> ExplosionForce;
};