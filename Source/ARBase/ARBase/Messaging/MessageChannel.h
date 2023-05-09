#pragma once

#include <ARBase/Messaging/Message.h>

#include <vector>

class UNetConnection;
class UChannelWrapper;

namespace ar
{

class ARBASE_API MessageChannel
{
public:
	struct ConnectionAdapter
	{
		TWeakObjectPtr<UNetConnection> NetConnection;
		TWeakObjectPtr<UChannelWrapper> NetMessageChannel;
	};

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

	friend class ::UChannelWrapper;
};

// MessageChannelRegistry --------------------------------------------------------------------------

struct MessageChannelRegistryEntry
{
	// Tracking data.
	const char* FromFile;
	int FromLine;
};
using MessageChannelRegistry = TMap<FName, MessageChannelRegistryEntry>;

ARBASE_API const MessageChannelRegistry& GetGlobalMessageChannelRegistry();
ARBASE_API const MessageChannelRegistryEntry*
FindMessageChannelRegistryEntry(const FName& channel_id);

namespace internal
{

class ARBASE_API __MessageChannelRegisterer
{
public:
	explicit __MessageChannelRegisterer(const FName& channel_id, const char* file, int line);
};

} // namespace internal

} // namespace ar
