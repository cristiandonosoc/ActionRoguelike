#pragma once

#define GENERATED_MESSAGE(class_name, parent_class_name)                                           \
public:                                                                                            \
	using Parent = parent_class_name;                                                              \
	static const ar::MessageType& GetMessageType();

#define DEFINE_MESSAGE(class_name)                                                                 \
	const ar::MessageType& class_name::GetMessageType()                                            \
	{                                                                                              \
		static ar::MessageType type{ TEXT(#class_name) };                                          \
		return type;                                                                               \
	}

namespace ar
{

struct MessageType;

class Message
{
public:
	static const MessageType& GetMessageType();

public:
	virtual ~Message() = default;

public:
	virtual void Serialize(FArchive& ar) = 0;
};

struct MessageType
{
	FName Id;
};

} // namespace ar
