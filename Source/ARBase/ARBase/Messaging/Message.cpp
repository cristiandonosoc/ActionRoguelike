#include <ARBase/Messaging/Message.h>

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

const MessageTypeRegistryEntry* FindMessageTypeRegistryEntry(const FName& type)
{
	const auto& registry = GetGlobalMessageTypeRegistry();
	return registry.Find(type);
}

namespace internal
{

__MessageTypeRegisterer::__MessageTypeRegisterer(
	const FName& type, const char* file, int line,
	MessageTypeRegistryEntry::MessageTypeFactoryFunction&& factory_function)
{
	auto& registry = GetMessageTypeRegistry();

	// This message type should not be registered twice.
	if (MessageTypeRegistryEntry* entry = registry.Find(type))
	{
		std::cout << "Message type " << TCHAR_TO_ANSI(*type.ToString())
				  << " already registered from " << entry->FromFile << ":" << entry->FromLine
				  << std::endl;

		// Put a couple so that all compilers will show the break line correctly.
		AR_DEBUGTRAP();
		AR_DEBUGTRAP();
		AR_DEBUGTRAP();
		return;
	}

	// We add it to the registry.
	MessageTypeRegistryEntry entry = {};
	entry.FactoryFunction = std::move(factory_function);
	entry.FromFile = file;
	entry.FromLine = line;
	registry[type] = std::move(entry);
}

} // namespace internal
} // namespace ar
