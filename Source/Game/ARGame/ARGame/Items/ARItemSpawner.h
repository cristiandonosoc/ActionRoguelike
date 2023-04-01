#pragma once

#include <ARGame/Gameplay/ARGameplayInterface.h>
#include <ARBase/ClientServerSplit.h>

#if AR_BUILD_CLIENT
#include <ARGameClient/Items/ARItemSpawnerClient.h>
#endif // AR_BUILD_CLIENT
#if AR_BUILD_SERVER
#include <ARGameServer/Items/ARItemSpawnerServer.h>
#endif // AR_BUILD_SERVER

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

#include "ARItemSpawner.generated.h"

class AARBaseItem;

class UCapsuleComponent;

// ItemSpawner is meant to have certain types of items on it and have them spawn and track when
// they need to be respawned or not.
UCLASS()
class ARGAME_API AARItemSpawner : public AActor, public IARInteractable
{
	GENERATED_BODY()
	GENERATED_BASE_CLIENT_SERVER_SPLIT(AARItemSpawner, ARItemSpawnerClient, ARItemSpawnerServer);

public:
	// Sets default values for this actor's properties
	AARItemSpawner();

public:
	float GetInitialDelay() const { return InitialDelay; }
	float GetRespawnDelay() const { return RespawnDelay; }
	const TSoftClassPtr<AARBaseItem>& GetItemClass() const { return ItemClass; }
	const TObjectPtr<AARBaseItem>& GetSpawnedItem() const { return SpawnedItem; }

	void SetSpawnedItem(const TObjectPtr<AARBaseItem>& item) { SpawnedItem = item; }

public:
	virtual void EndPlay(const EEndPlayReason::Type reason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& props) const override;

	// INTERFACE_BEGIN(IARInteractable)
	virtual bool CanInteract_Implementation(APawn* interactor) override;
	virtual bool Interact_Implementation(APawn* interactor) override;
	// INTERFACE_END(IARInteractable)

protected:
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* overlapped_component, AActor* other_actor,
						UPrimitiveComponent* other_comp, int32 other_body_index, bool from_sweep,
						const FHitResult& sweep_result);

	UFUNCTION()
	void OnBeginEnd(UPrimitiveComponent* overlapped_component, AActor* other_actor,
					UPrimitiveComponent* other_comp, int32 other_body_index);

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

	UPROPERTY(EditAnywhere)
	TObjectPtr<UShapeComponent> InteractionCollisionVolume;

private:
	UPROPERTY(Replicated)
	TObjectPtr<AARBaseItem> SpawnedItem;
	// Cache of the overlapping player pawn with this spawner.
	// We need to do this because if the player never leaves the collision, it will not be detected
	// as "entering" and therefore will not automatically interact with the object.
	//
	// With this we can track whether the player is "still there" once the object respawns.
	TWeakObjectPtr<APawn> CurrentlyOverlappingPlayerPawn;
};