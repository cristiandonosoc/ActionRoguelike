#pragma once

#ifdef AR_BUILD_CLIENT

#include <ARBase/ClientServerSplit.h>

struct FMessageChannelCreatedEventData;
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
	void OnChannelCreated(const FMessageChannelCreatedEventData& data);
};

} // namespace client
} // namespace ar

#endif // AR_BUILD_CLIENT
