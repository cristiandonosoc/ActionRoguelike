#include <ARBase/Messaging/Message.h>

namespace ar
{

std::unique_ptr<Message> Message::FactoryFromType(const FName& type)
{
	checkf(false, TEXT("TODO: IMPLEMENT ME"));
	return nullptr;
}


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

struct MessageTypeRegistryEntry
{
	__MessageTypeRegisterer::MessageTypeFactoryFunction FactoryFunction;

	// Tracking data.
	std::string FromFile;
	int FromLine;
};
using MessageTypeRegistry = TMap<FName, MessageTypeRegistryEntry>;

MessageTypeRegistry& GetMessageTypeRegistry()
{
	static MessageTypeRegistry registry = {};
	return registry;
}

} // namespace

__MessageTypeRegisterer::__MessageTypeRegisterer(const FName& type, const char* file, int line,
												 MessageTypeFactoryFunction&& factory_function)
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

} // namespace ar
