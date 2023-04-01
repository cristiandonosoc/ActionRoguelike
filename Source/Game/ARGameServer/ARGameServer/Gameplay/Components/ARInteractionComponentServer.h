#pragma once

#include <ARBase/ClientServerSplit.h>

class UARInteractionComponent;

class ARGAMESERVER_API ARInteractionComponentServer
{
	GENERATED_SERVER_SPLIT(UARInteractionComponent, ARInteractionComponentServer);

public:
	void Interact();
};