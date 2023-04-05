#pragma once

#include <ARBase/ClientServerSplit.h>

class AARBaseItem;
class AARItemSpawner;

class APawn;

namespace ar
{
namespace server
{

class ARGAMESERVER_API ItemSpawnerServer
{
	GENERATED_LEAF_SERVER_SPLIT(AARItemSpawner, ItemSpawnerServer);

public:
	void EndPlay();
	void ScheduleItemSpawning(float delay);
	void Interact(NotNullPtr<AARBaseItem> item, NotNullPtr<APawn> interactor);

private:
	void SpawnItem();

private:
	FTimerHandle SpawnTimerHandle;
};

} // namespace server
} // namespace ar