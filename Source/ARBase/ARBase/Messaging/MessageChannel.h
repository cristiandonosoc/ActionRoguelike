#pragma once

#include <ARBase/Messaging/Message.h>

#include <vector>

class UNetConnection;
class UNetMessageChannel;

namespace ar
{

class MessageChannel
{
public:
	struct ConnectionAdapter
	{
		TWeakObjectPtr<UNetConnection> NetConnection;
		TWeakObjectPtr<UNetMessageChannel> NetMessageChannel;
	};

public:
	MessageChannel();

public:
	const FName& GetId() const { return Id; }
	const std::vector<ConnectionAdapter>& GetConnections() const { return Connections; }

public:
	void Send(std::unique_ptr<Message>&& message, MessageDomain domain);

private:
	void ReceiveNetMessage(std::unique_ptr<Message>&& message);

private:
	FName Id;
	std::vector<ConnectionAdapter> Connections;

	friend class ::UNetMessageChannel;
};

} // namespace ar
