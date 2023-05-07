#include <ARBase/Messaging/Message.h>

namespace ar
{

DEFINE_MESSAGE(Message);

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

} // namespace ar
