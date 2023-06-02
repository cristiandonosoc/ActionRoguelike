#pragma once

#include <ARBase/Macros.h>

#include <unordered_map>

#define GENERATED_MESSAGE(class_name, parent_class_name)                                           \
public:                                                                                            \
	using Parent = parent_class_name;                                                              \
	static const MessageType& StaticMessageType();

#define DEFINE_MESSAGE(class_name)                                                                 \
	const MessageType& class_name::StaticMessageType()                                             \
	{                                                                                              \
		static MessageType type{TEXT(#class_name)};                                                \
		return type;                                                                               \
	}                                                                                              \
	static ::ar::__MessageTypeRegisterer AR_CONCAT3(__message_type_registerer__, class_name,       \
													__LINE__)(                                     \
		class_name::StaticMessageType(), class_name::Parent::StaticMessageType(), __FILE__,        \
		__LINE__,                                                                                  \
		[]()                                                                                       \
		{                                                                                          \
			auto message = std::make_unique<class_name>();                                         \
			message->SetType(class_name::StaticMessageType());                              \
			return message;                                                                        \
		});

namespace ar
{

// MessageDomain represents where this message will be sent to.
enum class MessageDomain : uint8
{
	Local,
	Remote,
	Both,
};
ARBASE_API const char* ToString(MessageDomain domain);

struct ARBASE_API MessageType
{
	FName ID;

	// So that we can hash this as an hash for std containers.
	bool operator==(const MessageType& other) const;
	bool operator!=(const MessageType& other) const;
	std::size_t operator()(const MessageType& type) const noexcept;
};

class ARBASE_API Message
{
public:
	virtual ~Message() = default;

public:
	static const MessageType& StaticMessageType();

public:
	void SetType(const MessageType& type) { Type = type; }
	const MessageType& GetType() const { return Type; }

public:
	virtual void Serialize(FArchive& ar) = 0;

protected:
	MessageType Type;
};

// MessageTypeRegistry -----------------------------------------------------------------------------

struct MessageTypeRegistryEntry
{
	using MessageTypeFactoryFunction = std::function<std::unique_ptr<Message>()>;

	MessageType Type;
	MessageType ParentType;
	MessageTypeFactoryFunction FactoryFunction;

	// Tracking data.
	const char* FromFile;
	int FromLine;
};
using MessageTypeRegistry = std::unordered_map<MessageType, MessageTypeRegistryEntry, MessageType>;

ARBASE_API const MessageTypeRegistry& GetGlobalMessageTypeRegistry();
ARBASE_API const MessageTypeRegistryEntry& FindMessageTypeRegistryEntry(const FName& id);

class ARBASE_API __MessageTypeRegisterer
{
public:
	explicit __MessageTypeRegisterer(
		const MessageType& type, const MessageType& parent_type, const char* file, int line,
		MessageTypeRegistryEntry::MessageTypeFactoryFunction&& factory_function);
};

} // namespace ar