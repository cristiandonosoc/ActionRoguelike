#include <ARBase/Messaging/MessageChannel.h>

#include <ARBase/Messaging/NetMessageChannel.h>
#include <ARBase/NotNullPtr.h>

namespace ar
{

namespace
{

void DispatchRemote(NotNullPtr<MessageChannel> channel, std::unique_ptr<Message>&& message);

} // namespace

void Dispatch(NotNullPtr<MessageChannel> channel, std::unique_ptr<Message>&& message,
			  MessageDomain domain)
{
	checkf(domain == MessageDomain::Remote, TEXT("Only Remote supported for now"));
	DispatchRemote(channel, std::move(message));
}

namespace
{
void DispatchRemote(NotNullPtr<MessageChannel> channel, std::unique_ptr<Message>&& message)
{
	// We iterate over all the connections.
	for (auto& connection : channel->Connections)
	{
		NotNullPtr<UNetMessageChannel> uchannel = connection.NetMessageChannel.Get();
		uchannel->Enqueue(std::move(message));
	}
}

} // namespace

const char* ToString(MessageDomain domain)
{
	// clang-format off
	switch (domain)
	{
	case MessageDomain::Local: return "Local";
	case MessageDomain::Remote: return "Remote";
	case MessageDomain::Both: return "Both";
	}
	// clang-format on

	checkf(false, TEXT("Invalid domain given %d"), domain);
	return "<invalid>";
}


} // namespace ar
