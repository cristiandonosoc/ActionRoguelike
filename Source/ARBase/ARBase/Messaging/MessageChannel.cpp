#include <ARBase/Messaging/MessageChannel.h>

#include <ARBase/Messaging/NetMessageChannel.h>
#include <ARBase/NotNullPtr.h>

namespace ar
{


MessageChannel::MessageChannel() {}

namespace
{

void DispatchRemote(NotNullPtr<MessageChannel> channel, std::unique_ptr<Message>&& message)
{
	// We iterate over all the connections.
	for (auto& connection : channel->GetConnections())
	{
		NotNullPtr<UNetMessageChannel> uchannel = connection.NetMessageChannel.Get();
		uchannel->Enqueue(std::move(message));
	}
}

} // namespace

void MessageChannel::Send(std::unique_ptr<Message>&& message, MessageDomain domain)
{
	checkf(domain == MessageDomain::Remote, TEXT("Only Remote supported for now"));
	DispatchRemote(this, std::move(message));
}

void MessageChannel::ReceiveNetMessage(std::unique_ptr<Message>&& message)
{
	checkf(false, TEXT("TODO: IMPLEMENT ME"));
}

} // namespace ar
