// Fill out your copyright notice in the Description page of Project Settings.

#include <ARGame/Items/ARItemSpawner.h>

#include <ARBase/NotNullPtr.h>
#include <ARBase/Subsystems/ARStreamingSubsystem.h>
#include <ARGame/Items/ARBaseItem.h>

#include <Components/CapsuleComponent.h>

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
	CollisionCapsule->OnComponentEndOverlap.AddDynamic(this, &AARItemSpawner::OnBeginEnd);
}

// Called when the game starts or when spawned
void AARItemSpawner::BeginPlay()
{
	Super::BeginPlay();

	// TODO(cdc): Do async loading.
	NotNullPtr streamer = GetGameInstance()->GetSubsystem<UARStreamingSubsystem>();
	streamer->RequestSyncLoad(ItemClass);

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
	if (!pawn || pawn != other_actor)
	{
		return;
	}

	// We track the pawn that is overlapping.
	CurrentlyOverlappingPlayerPawn = pawn;
	Interact_Implementation(pawn);
}

void AARItemSpawner::OnBeginEnd(UPrimitiveComponent* overlapped_component, AActor* other_actor,
								UPrimitiveComponent* other_comp, int32 other_body_index)
{

	// See if the player is leaving, so that we can stop tracking this.
	NotNullPtr<APlayerController> pc = GetWorld()->GetFirstPlayerController();
	APawn* pawn = pc->GetPawn();
	if (!pawn || pawn != other_actor)
	{
		return;
	}

	// The player left our overlap, so we stop tracking it.
	CurrentlyOverlappingPlayerPawn = nullptr;
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

	// We check if the player is currently overlapping, so that we trigger immediately the
	// interaction.
	if (APawn* player_pawn = CurrentlyOverlappingPlayerPawn.Get())
	{
		Interact_Implementation(player_pawn);
	}
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
	check(interactor);

	// If there is no spawned item, we don't want to count as interacting.
	if (!SpawnedItem)
	{
		return false;
	}

	// Check if the item can interact with this caller.
	if (!SpawnedItem->CanUse(interactor))
	{
		return false;
	}

	// We use the object and we destroy it.
	SpawnedItem->Use(interactor);
	SpawnedItem->Destroy();
	SpawnedItem = nullptr;

	ScheduleItemSpawning(RespawnDelay);

	return true;
}