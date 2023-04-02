#pragma once

#include <ARBase/ClientServerSplit.h>

class UARInteractionComponent;

namespace ar
{
namespace server
{

class ARGAMESERVER_API InteractionComponentServer
{
	GENERATED_SERVER_SPLIT(UARInteractionComponent);

public:
	void Interact();
};

} // namespace server
} // namespace ar