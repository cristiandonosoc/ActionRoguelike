#include <ARBase/Messaging/MessageEndpoint.h>

#include <ARBase/Messaging/MessageChannel.h>
#include <ARBase/NotNullPtr.h>

#include <iostream>

// DEFINE_MESSAGE_ENDPOINT(TestMessageEndpoint);

namespace ar
{
namespace
{

void DispatchRemote(NotNullPtr<MessageEndpoint> endpoint, std::unique_ptr<Message>&& message)
{
	// We iterate over all the UMessageChannel associated with this endpoint.
	for (const auto& weakChannel : endpoint->GetMessageChannels())
	{
		NotNullPtr<UMessageChannel> channel = weakChannel.Get();
		channel->Enqueue(std::move(message));
	}
}

} // namespace

void MessageEndpoint::Send(std::unique_ptr<Message>&& message, MessageDomain domain)
{
	checkf(domain == MessageDomain::Remote, TEXT("Only Remote supported for now"));
	DispatchRemote(this, std::move(message));
}

void MessageEndpoint::ReceiveMessageFromChannel(NotNullPtr<UMessageChannel> channel,
												std::unique_ptr<Message>&& message)
{
	checkf(false, TEXT("TODO: IMPLEMENT ME"));
}

// MessageEndpointRegistry -------------------------------------------------------------------------

namespace
{

MessageEndpointRegistry& GetStaticMessageEndpointRegistry()
{
	static MessageEndpointRegistry registry = {};
	return registry;
}

} // namespace

const MessageEndpointRegistry& GetGlobalMessageEndpointRegistry()
{
	return GetStaticMessageEndpointRegistry();
}

const MessageEndpointRegistryEntry* FindMessageEndpointRegistryEntry(const FName& endpoint_id)
{
	const auto& registry = GetGlobalMessageEndpointRegistry();
	return registry.Find(endpoint_id);
}

namespace internal
{

__MessageEndpointRegisterer::__MessageEndpointRegisterer(const FName& endpoint_id, const char* file,
														 int line)
{
	auto& registry = GetStaticMessageEndpointRegistry();

	// This channel id should not be registered twice.
	if (auto* entry = registry.Find(endpoint_id))
	{
		std::cout << "Channel " << TCHAR_TO_ANSI(*endpoint_id.ToString())
				  << " already registered at " << entry->FromFile << ":" << entry->FromLine
				  << std::endl;

		// Put a couple so that all compilers will show the break line correctly.
		AR_DEBUGTRAP();
		AR_DEBUGTRAP();
		AR_DEBUGTRAP();
		return;
	}

	// We add it to the registry.
	MessageEndpointRegistryEntry entry = {};
	entry.EndpointId = endpoint_id;
	entry.FromFile = file;
	entry.FromLine = line;
	registry[endpoint_id] = std::move(entry);
}

} // namespace internal
} // namespace ar