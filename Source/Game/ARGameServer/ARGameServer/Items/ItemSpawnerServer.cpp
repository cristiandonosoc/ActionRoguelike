#include <ARGameServer/Items/ItemSpawnerServer.h>

#include <ARGame/Items/ARBaseItem.h>
#include <ARGame/Items/ARItemSpawner.h>

namespace ar
{
namespace server
{

void ItemSpawnerServer::EndPlay()
{
	// Clear any pending timer.
	GetBase()->GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
}

void ItemSpawnerServer::ScheduleItemSpawning(float delay)
{
	check(!GetBase()->GetSpawnedItem());

	// We check if we need to spawn the object immediately.
	if (delay == 0.0f)
	{
		SpawnItem();
	}

	// Otherwise, we schedule it for spawning.
	FTimerDelegate delegate;
	delegate.BindRaw(this, &ItemSpawnerServer::SpawnItem);
	GetBase()->GetWorldTimerManager().SetTimer(SpawnTimerHandle, std::move(delegate), delay, false);
}

void ItemSpawnerServer::Interact(NotNullPtr<AARBaseItem> item, NotNullPtr<APawn> interactor)
{
	// We use the object and we destroy it.
	item->Use(interactor);
	item->Destroy();
	GetBase()->SetSpawnedItem(nullptr);

	// Schedule the item to re-appear.
	ScheduleItemSpawning(GetBase()->GetRespawnDelay());
}

void ItemSpawnerServer::SpawnItem()
{
	const auto& item_class = GetBase()->GetItemClass();
	if (!ensure(item_class.IsValid()))
	{
		return;
	}

	// There should not be an item present.
	check(!GetBase()->GetSpawnedItem());

	// Spawn the item.
	// This will replicate to the other side.
	FActorSpawnParameters params = {};
	params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	NotNullPtr<AARBaseItem> item = GetBase()->GetWorld()->SpawnActor<AARBaseItem>(
		item_class.Get(), GetBase()->GetActorTransform(), params);
	GetBase()->SetSpawnedItem(item);

	// // We check if the player is currently overlapping, so that we trigger immediately the
	// // interaction.
	// if (APawn* player_pawn = CurrentlyOverlappingPlayerPawn.Get())
	// {
	// 	IARInteractable::Execute_Interact(this, player_pawn);
	// }
}

} // namespace server
} // namespace ar
