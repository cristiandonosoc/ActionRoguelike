#include <ARBase/Messaging/MessageChannel.h>

#include <ARBase/Messaging/NetMessageChannel.h>
#include <ARBase/NotNullPtr.h>

namespace ar
{
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

// MessageChannelRegistry --------------------------------------------------------------------------

namespace
{

MessageChannelRegistry& GetStaticMessageChannelRegistry()
{
	static MessageChannelRegistry registry = {};
	return registry;
}

} // namespace

const MessageChannelRegistry& GetGlobalMessageChannelRegistry()
{
	return GetStaticMessageChannelRegistry();
}

const MessageChannelRegistryEntry* FindMessageChannelRegistryEntry(const FName& channel_id)
{
	const auto& registry = GetGlobalMessageChannelRegistry();
	return registry.Find(channel_id);
}

namespace internal
{
__MessageChannelRegisterer::__MessageChannelRegisterer(const FName& channel_id, const char* file,
													   int line)
{
	auto& registry = GetStaticMessageChannelRegistry();

	// This channel id should not be registered twice.
	if (auto* entry = registry.Find(channel_id))
	{
		std::cout << "Channel " << TCHAR_TO_ANSI(*channel_id.ToString())
				  << " already registered at " << entry->FromFile << ":" << entry->FromLine
				  << std::endl;

		// Put a couple so that all compilers will show the break line correctly.
		AR_DEBUGTRAP();
		AR_DEBUGTRAP();
		AR_DEBUGTRAP();
		return;
	}

	// We add it to the registry.
	MessageChannelRegistryEntry entry = {};
	entry.FromFile = file;
	entry.FromLine = line;
	registry[channel_id] = std::move(entry);
}

} // namespace internal
} // namespace ar
