// Fill out your copyright notice in the Description page of Project Settings.


#include "ARItemSpawner.h"


#include "ARBase/NotNullPtr.h"
#include "ARBaseItem.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AARItemSpawner::AARItemSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if
	// you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>("CollisionCapsule");
	RootComponent = CollisionCapsule;
}

void AARItemSpawner::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AARItemSpawner::OnBeginOverlap);
	//CollisionCapsule->OnComponentEndOverlap.AddDynamic(this, &AARItemSpawner::OnEndOverlap);
}

// Called when the game starts or when spawned
void AARItemSpawner::BeginPlay()
{
	Super::BeginPlay();

	// We schedule the item to be spawned.
	ScheduleItemSpawning(InitialDelay);
}

// Called every frame
void AARItemSpawner::Tick(float delta)
{
	Super::Tick(delta);
}

void AARItemSpawner::OnBeginOverlap(UPrimitiveComponent* overlapped_component, AActor* other_actor,
									UPrimitiveComponent* other_comp, int32 other_body_index,
									bool from_sweep, const FHitResult& sweep_result)
{
	// If there is no item, we simply don't interact.
	if (!SpawnedItem)
	{
		return;
	}

	// Check if this is the player colliding with the spawner.
	NotNullPtr<APlayerController> pc = GetWorld()->GetFirstPlayerController();
	APawn* pawn = pc->GetPawn();
	if (!pawn)
	{
		return;
	}

	if (pawn != other_actor)
	{
		return;
	}

	Interact_Implementation(pawn);
}
void AARItemSpawner::OnEndOverlap(UPrimitiveComponent* overlapped_component, AActor* other_actor,
								  UPrimitiveComponent* other_comp, int32 other_body_index)
{
	// The item should be destroyed by now.
	//check(!SpawnedItem);

	// We schedule the next version of the item to be spawned.
	//ScheduleItemSpawning(RespawnDelay);
}

void AARItemSpawner::SpawnItem()
{
	bool is_set = !ItemClass.IsNull();
	if (!ensure(is_set))
	{
		return;
	}

	check(!SpawnedItem); // There should not be an item present.

	FActorSpawnParameters params = {};
	params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	NotNullPtr<AARBaseItem> item =
		GetWorld()->SpawnActor<AARBaseItem>(ItemClass.Get(), GetActorTransform(), params);
	SpawnedItem = item.Get();
}

void AARItemSpawner::ScheduleItemSpawning(float delay)
{
	check(!SpawnedItem);

	// We check if we need to spawn the object immediately.
	if (delay == 0.0f)
	{
		SpawnItem();
	}

	// Otherwise, we schedule it for spawning.
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AARItemSpawner::SpawnItem, delay,
									false);
}


bool AARItemSpawner::Interact_Implementation(APawn* interactor)
{
	// If there is no spawned item, we don't want to count as interacting.
	if (!SpawnedItem)
	{
		return false;
	}

	// We use the object and we destroy it.
	SpawnedItem->TriggerUse(interactor);
	SpawnedItem->Destroy();
	SpawnedItem = nullptr;

	ScheduleItemSpawning(RespawnDelay);

	return true;
}