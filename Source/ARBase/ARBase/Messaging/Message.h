#pragma once

#define GENERATED_MESSAGE(class_name, parent_class_name)                                           \
public:                                                                                            \
	using Parent = parent_class_name;                                                              \
	static const FName& StaticMessageType();

#define DEFINE_MESSAGE(class_name)                                                                 \
	const FName& class_name::StaticMessageType()                                                   \
	{                                                                                              \
		static FName type{ TEXT(#class_name) };                                                    \
		return type;                                                                               \
	}

namespace ar
{

// MessageDomain represents where this message will be sent to.
enum class MessageDomain : uint8
{
	Local,
	Remote,
	Both,
};
const char* ToString(MessageDomain domain);

class Message
{
public:
	static const FName& StaticMessageType();
	static std::unique_ptr<Message> FactoryFromType(const FName& type);

public:
	virtual ~Message() = default;

public:
	const FName& GetMessageType() const { return MessageType; }

public:
	virtual void Serialize(FArchive& ar) = 0;

protected:
	FName MessageType;
};


} // namespace ar
