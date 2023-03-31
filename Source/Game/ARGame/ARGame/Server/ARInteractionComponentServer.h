#pragma once
#include <ARBase/ClientServerSplit.h>
#ifdef AR_BUILD_SERVER

class UARInteractionComponent;

class ARInteractionComponentServer
{
	GENERATED_SERVER_SPLIT(UARInteractionComponent, ARInteractionComponentServer);

public:
	void Interact();
};

#endif // AR_BUILD_SERVER