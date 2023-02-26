// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ARGame/ARGameplayInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ARItemSpawner.generated.h"

class AARBaseItem;

class UCapsuleComponent;

// ItemSpawner is meant to have certain types of items on it and have them spawn and track when
// they need to be respawned or not.
UCLASS()
class ARGAME_API AARItemSpawner : public AActor, public IARGameplayInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AARItemSpawner();

	virtual void Tick(float delta) override;

	virtual bool Interact_Implementation(APawn* interactor) override;

protected:
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* overlapped_component, AActor* other_actor,
						UPrimitiveComponent* other_comp, int32 other_body_index, bool from_sweep,
						const FHitResult& sweep_result);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* overlapped_component, AActor* other_actor,
					  UPrimitiveComponent* other_comp, int32 other_body_index);
private:
	UFUNCTION()
	void SpawnItem();
	void ScheduleItemSpawning(float delay);

protected:
	// Item tracks the object to spawn in this spawner.
	UPROPERTY(EditAnywhere, Category = "Item")
	TSoftClassPtr<AARBaseItem> ItemClass;

	// InitialDelay is how much the spawner will wait after begin play to spawn the item.
	UPROPERTY(EditAnywhere, Category = "Item")
	float InitialDelay = 0.0f;

	// RespawnDelay is how much the spawner will wait to spawn the item *after* the player has left
	// the collision.
	UPROPERTY(EditAnywhere, Category = "Item")
	float RespawnDelay = 1.0f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCapsuleComponent> CollisionCapsule;

private:
	UPROPERTY()
	TObjectPtr<AARBaseItem> SpawnedItem;

	FTimerHandle SpawnTimerHandle;
};