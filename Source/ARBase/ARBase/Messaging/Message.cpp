#include <ARBase/Messaging/Message.h>

#include <iostream>

namespace ar
{

class TestMessage : public Message
{
	GENERATED_MESSAGE(TestMessage, Message);

public:
	void Serialize(FArchive& ar) override {}
};

DEFINE_MESSAGE(TestMessage);

} // namespace ar

namespace ar
{

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

bool MessageType::operator==(const MessageType& other) const
{
	return ID == other.ID;
}

bool MessageType::operator!=(const MessageType& other) const
{
	return !operator==(other);
}

std::size_t MessageType::operator()(const MessageType& type) const noexcept
{
	return GetTypeHash(type.ID);
}

const MessageType& Message::StaticMessageType()
{
	static MessageType type{TEXT("Message")};
	return type;
}

// __MessageTypeRegisterer -------------------------------------------------------------------------

namespace
{

MessageTypeRegistry& GetMessageTypeRegistry()
{
	static MessageTypeRegistry registry = {};
	return registry;
}

} // namespace

const MessageTypeRegistry& GetGlobalMessageTypeRegistry()
{
	return GetMessageTypeRegistry();
}

const MessageTypeRegistryEntry& FindMessageTypeRegistryEntry(const FName& id)
{
	const auto& registry = GetGlobalMessageTypeRegistry();

	MessageType dummy{id};
	auto it = registry.find(dummy);
	check(it != registry.end());
	return it->second;
}

__MessageTypeRegisterer::__MessageTypeRegisterer(
	const MessageType& type, const MessageType& parent_type, const char* file, int line,
	MessageTypeRegistryEntry::MessageTypeFactoryFunction&& factory_function)
{
	auto& registry = GetMessageTypeRegistry();

	// This message type should not be registered twice.
	if (auto it = registry.find(type); it != registry.end())
	{
		const auto& [_, entry] = *it;

		std::cout << "Message type " << TCHAR_TO_ANSI(*type.ID.ToString())
				  << " already registered from " << entry.FromFile << ":" << entry.FromLine
				  << std::endl;

		// Put a couple so that all compilers will show the break line correctly.
		AR_DEBUGTRAP();
		AR_DEBUGTRAP();
		AR_DEBUGTRAP();
		return;
	}

	// We add it to the registry.
	MessageTypeRegistryEntry entry = {};
	entry.Type = type;
	entry.ParentType = parent_type;
	entry.FactoryFunction = std::move(factory_function);
	entry.FromFile = file;
	entry.FromLine = line;
	registry[type] = std::move(entry);
}

} // namespace ar