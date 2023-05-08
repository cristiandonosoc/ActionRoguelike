#pragma once

#ifdef AR_BUILD_SERVER

#include <ARBase/ClientServerSplit.h>

class UARGameInstance;

namespace ar
{

class MessagingManager;

namespace server
{

class MessagingManagerServer
{
	GENERATED_LEAF_SERVER_SPLIT(ar::MessagingManager, ar::server::MessagingManagerServer);

public:
	void Start();

private:
	UARGameInstance* GetGameInstance();
};

} // namespace server
} // namespace ar

#endif // AR_BUILD_SERVER
