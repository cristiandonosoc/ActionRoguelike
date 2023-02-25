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
}

// Called when the game starts or when spawned
void AARItemSpawner::BeginPlay()
{
	Super::BeginPlay();

	// If there is no delay, we spawn the item immediately.
	// Otherwise, we add a timer.
	if (InitialDelay == 0.0f)
	{
		SpawnItem();
		return;
	}

	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AARItemSpawner::SpawnItem,
									InitialDelay, false);
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
void AARItemSpawner::SpawnItem()
{
	if (!ensure(ItemClass))
	{
		return;
	}
	
	check(!SpawnedItem);	// There should not be an item present.

	FActorSpawnParameters params = {};
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	NotNullPtr<AARBaseItem> item = GetWorld()->SpawnActor<AARBaseItem>(ItemClass.Get(), GetActorTransform(), params);
	SpawnedItem = item.Get();
}

bool AARItemSpawner::Interact_Implementation(APawn* interactor)
{
	// If there is no spawned item, we don't want to count as interacting.
	if (SpawnedItem)
	{
		return false;
	}

	
	UE_LOG(LogTemp, Log, TEXT("INTERACTING!"));

	return true;
}