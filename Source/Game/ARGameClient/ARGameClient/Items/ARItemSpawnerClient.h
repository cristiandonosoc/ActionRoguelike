#pragma once

#include <ARBase/ClientServerSplit.h>


class AARBaseItem;
class ARItemSpawner;

class ARGAMECLIENT_API ARItemSpawnerClient
{
	GENERATED_CLIENT_SPLIT(AARItemSpawner, ARItemSpawnerClient);

public:
	bool Interact(NotNullPtr<AARBaseItem> item, NotNullPtr<APawn> interactor);
	
};
