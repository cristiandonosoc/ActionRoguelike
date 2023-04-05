#pragma once

#include <ARBase/ClientServerSplit.h>

class UARInteractionComponent;

namespace ar
{
namespace server
{

class ARGAMESERVER_API InteractionComponentServer
{
	GENERATED_LEAF_SERVER_SPLIT(UARInteractionComponent, InteractionComponentServer);

public:
	void Interact();
};

} // namespace server
} // namespace ar