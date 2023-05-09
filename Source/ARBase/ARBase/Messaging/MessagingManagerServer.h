#pragma once

#ifdef AR_BUILD_SERVER

#include <ARBase/ClientServerSplit.h>

class AARPlayerController;
class UARGameInstance;

namespace ar
{

class MessagingManager;

namespace server
{

class ARBASE_API MessagingManagerServer
{
	GENERATED_LEAF_SERVER_SPLIT(ar::MessagingManager, ar::server::MessagingManagerServer);

public:
	void OnNewConnection(NotNullPtr<AARPlayerController> player_controller,
						 NotNullPtr<UNetConnection> connection);

private:
	UARGameInstance* GetGameInstance();
};

} // namespace server
} // namespace ar

#endif // AR_BUILD_SERVER
