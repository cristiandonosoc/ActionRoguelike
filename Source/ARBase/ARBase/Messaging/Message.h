#pragma once

#define GENERATED_MESSAGE(class_name, parent_class_name)                                           \
public:                                                                                            \
	using Parent = parent_class_name;                                                              \
	static const ar::Message::Type& StaticMessageType();

#define DEFINE_MESSAGE(class_name)                                                                 \
	const ar::Message::Type& class_name::StaticMessageType()                                          \
	{                                                                                              \
		static ar::Message::Type type{ TEXT(#class_name) };                                        \
		return type;                                                                               \
	}

namespace ar
{

class Message
{
public:
	// Type is an entry in a bare bones type system to identity a message.
	struct Type
	{
		FName Id;
	};

public:
	static const Type& StaticMessageType();

public:
	virtual ~Message() = default;

public:
	const Type& GetMessageType() const { return MessageType; }

public:
	virtual void Serialize(FArchive& ar) = 0;

protected:
	Type MessageType = {};
};


} // namespace ar
