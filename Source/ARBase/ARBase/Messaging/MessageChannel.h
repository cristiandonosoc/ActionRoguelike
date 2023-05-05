#pragma once

#include <ARBase/Messaging/Message.h>

#include <vector>

class UNetConnection;
class UNetMessageChannel;

namespace ar
{

using MessageChannelId = FName;

struct MessageChannel
{
	struct ConnectionAdapter
	{
		TWeakObjectPtr<UNetConnection> NetConnection;
		TWeakObjectPtr<UNetMessageChannel> NetMessageChannel;
	};

	MessageChannelId Id;
	std::vector<ConnectionAdapter> Connections;
};

void Dispatch(MessageChannel* channel, std::unique_ptr<Message>&& message, MessageDomain domain);

} // namespace ar
