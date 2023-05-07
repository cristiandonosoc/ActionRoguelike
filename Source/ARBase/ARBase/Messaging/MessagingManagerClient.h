#pragma once

#ifdef AR_BUILD_CLIENT

#include <ARBase/ClientServerSplit.h>

namespace ar
{

class MessagingManager;

namespace client
{

class MessagingManagerClient
{
	GENERATED_LEAF_CLIENT_SPLIT(ar::MessagingManager, MessagingManagerClient);
};

} // namespace client
} // namespace ar

#endif // AR_BUILD_CLIENT
