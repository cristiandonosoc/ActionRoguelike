#include <ARGame/Items/ARItemSpawner.h>

#include <ARBase/NotNullPtr.h>
#include <ARBase/Subsystems/ARStreamingSubsystem.h>
#include <ARGame/Items/ARBaseItem.h>

#include <Components/CapsuleComponent.h>
#include <Net/UnrealNetwork.h>

// Sets default values
AARItemSpawner::AARItemSpawner()
{
	INIT_BASE_CLIENT_SERVER_SPLIT();
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
	SERVER_ONLY_CALL(ScheduleItemSpawning, InitialDelay);
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
	IARInteractable::Execute_Interact(this, pawn);
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

void AARItemSpawner::EndPlay(const EEndPlayReason::Type reason)
{
	SERVER_ONLY_CALL(EndPlay);
	
	Super::EndPlay(reason);
}

void AARItemSpawner::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& props) const
{
	Super::GetLifetimeReplicatedProps(props);

	// Unreal expects this stupid ass name.
	auto& OutLifetimeProps = props;
	DOREPLIFETIME(AARItemSpawner, SpawnedItem);
}

bool AARItemSpawner::CanInteract_Implementation(APawn* interactor)
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

	return true;
}

bool AARItemSpawner::Interact_Implementation(APawn* interactor)
{
	// We verify that we can interact.
	if (!IARInteractable::Execute_CanInteract(this, interactor))
	{
		return false;
	}

	// The item via replication will
	CLIENT_SERVER_CALL(Interact, SpawnedItem.Get(), interactor);
	return true;
}