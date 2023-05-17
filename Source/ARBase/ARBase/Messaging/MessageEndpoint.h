#pragma once

#include <ARBase/Messaging/Message.h>

#include <ARBase/NotNullPtr.h>
#include <vector>

class UNetConnection;
class UMessageChannel;

namespace ar
{

// MessageEndpoint is the high-level abstraction that gameplay code will use to send and receive
// messages. MessageEndpoints can be local (in which they route messages only within a particular
// client or server), or remote, in which they will send their message to their connected
// counterpart (or potentially counterparts, in the case of the server).
//
// Remote (networked) Endpoints are associated with a particular |UMessageChannel|, which are also
// bound to a particular |NetConnection|.
class ARBASE_API MessageEndpoint
{
public:
public:
	const FName& GetId() const { return Id; }
	const auto& GetMessageChannels() const { return MessageChannels; }

public:
	void Send(std::unique_ptr<Message>&& message, MessageDomain domain);

private:
	void ReceiveMessageFromChannel(NotNullPtr<UMessageChannel> channel,
								   std::unique_ptr<Message>&& message);

private:
	FName Id;
	std::vector<TWeakObjectPtr<UMessageChannel>> MessageChannels;

private:
	// So that we can only receive messages being decoded from a channel.
	friend ::UMessageChannel;
};

// MessageEndpointRegistry -------------------------------------------------------------------------

struct MessageEndpointRegistryEntry
{
	// Tracking data.
	const char* FromFile;
	int FromLine;
};
using MessageEndpointRegistry = TMap<FName, MessageEndpointRegistryEntry>;

ARBASE_API const MessageEndpointRegistry& GetGlobalMessageEndpointRegistry();
ARBASE_API const MessageEndpointRegistryEntry*
FindMessageEndpointRegistryEntry(const FName& endpoint_id);

namespace internal
{

class ARBASE_API __MessageEndpointRegisterer
{
public:
	explicit __MessageEndpointRegisterer(const FName& endpoint_id, const char* file, int line);
};

} // namespace internal

} // namespace ar