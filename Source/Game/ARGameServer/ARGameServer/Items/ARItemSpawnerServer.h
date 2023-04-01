#pragma once

#include <ARBase/ClientServerSplit.h>

class AARBaseItem;
class ARItemSpawner;

class APawn;

class ARGAMESERVER_API ARItemSpawnerServer
{
	GENERATED_SERVER_SPLIT(AARItemSpawner, ARItemSpawnerServer);

public:
	void EndPlay();
	void ScheduleItemSpawning(float delay);
	void Interact(NotNullPtr<AARBaseItem> item, NotNullPtr<APawn> interactor);

private:
	void SpawnItem();

private:

	FTimerHandle SpawnTimerHandle;
};
