#pragma once

#ifdef AR_BUILD_SERVER

#include <ARBase/ClientServerSplit.h>

namespace ar
{

class MessagingManager;

namespace server
{

class MessagingManagerServer
{
	GENERATED_LEAF_SERVER_SPLIT(ar::MessagingManager, ar::server::MessagingManagerServer);
};

} // namespace server
} // namespace ar

#endif // AR_BUILD_SERVER
