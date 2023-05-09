#pragma once

#ifdef AR_BUILD_CLIENT

#include <ARBase/ClientServerSplit.h>

class AARPlayerController;

namespace ar
{

class MessagingManager;

namespace client
{

class ARBASE_API MessagingManagerClient
{
	GENERATED_LEAF_CLIENT_SPLIT(ar::MessagingManager, MessagingManagerClient);

public:
	void OnNewConnection(NotNullPtr<AARPlayerController> player_controller,
						 NotNullPtr<UNetConnection> connection);
};

} // namespace client
} // namespace ar

#endif // AR_BUILD_CLIENT
