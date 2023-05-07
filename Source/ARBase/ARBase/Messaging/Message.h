#pragma once

#include <ARBase/Macros.h>

#define GENERATED_MESSAGE(class_name, parent_class_name)                                           \
public:                                                                                            \
	using Parent = parent_class_name;                                                              \
	static const FName& StaticMessageType();

#define DEFINE_MESSAGE(class_name)                                                                 \
	const FName& class_name::StaticMessageType()                                                   \
	{                                                                                              \
		static FName type{ TEXT(#class_name) };                                                    \
		return type;                                                                               \
	}                                                                                              \
	static ::ar::__MessageTypeRegisterer AR_CONCAT3(__message_type_registerer__, class_name,       \
													__LINE__)(                                     \
		class_name::StaticMessageType(), __FILE__, __LINE__,                                       \
		[]() { return std::make_unique<class_name>(); });

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

class ARBASE_API Message
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

class ARBASE_API __MessageTypeRegisterer
{
public:
	using MessageTypeFactoryFunction = std::function<std::unique_ptr<Message>()>;

	explicit __MessageTypeRegisterer(const FName& type, const char* file, int line,
									 MessageTypeFactoryFunction&& factory_function);
};


} // namespace ar
