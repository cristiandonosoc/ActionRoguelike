#pragma once

#include <vector>

class UNetConnection;
class UNetMessageChannel;

namespace ar
{

class Message;

enum class MessageDomain
{
		Local,
		Remote,
		Both,
};
const char* ToString(MessageDomain domain);

struct MessageChannel
{
	using IDType = FName;

	struct ConnectionAdapter
	{
		TWeakObjectPtr<UNetConnection> NetConnection;
		TWeakObjectPtr<UNetMessageChannel> NetMessageChannel;
	};
	std::vector<ConnectionAdapter> Connections;
};

void Dispatch(MessageChannel* channel, std::unique_ptr<Message>&& message, MessageDomain domain);

} // namespace ar
