#pragma once

#include <ARBase/ClientServerSplit.h>

class AARBaseItem;
class AARItemSpawner;

namespace ar
{
namespace client
{

class ARGAMECLIENT_API ItemSpawnerClient
{
	GENERATED_LEAF_CLIENT_SPLIT(AARItemSpawner);

public:
	bool Interact(NotNullPtr<AARBaseItem> item, NotNullPtr<APawn> interactor);
	
};

} // namespace client
} // namespace ar
