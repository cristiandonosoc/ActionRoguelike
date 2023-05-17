#include <ARBase/Messaging/MessageChannel.h>

#include <ARBase/DebugTools.h>
#include <ARBase/Logging.h>
#include <ARBase/Messaging/Message.h>

void UMessageChannel::Enqueue(std::unique_ptr<ar::Message>&& message)
{
	check(message);
	checkf(!Closing, TEXT("Channel %s, MessageType %s: Enqueing to channel while closing"),
		   *GetName(), *message->GetMessageType().ToString());
	MessageQueue.push_back(std::move(message));
}

void UMessageChannel::Init(UNetConnection* in_connection, int32 in_ch_index,
						   EChannelCreateFlags create_flags)
{
	Super::Init(in_connection, in_ch_index, create_flags);
	AR_LOG_CSS(GetWorld(), LogARBase_Messaging, Log, TEXT("Initializing channel %s"), *GetName());
}

bool UMessageChannel::CleanUp(const bool for_destroy, EChannelCloseReason close_reason)
{
	checkf(false, TEXT("TODO: IMPLEMENT ME"));

	if (!ensure(MessageQueue.empty()))
	{
		AR_LOG_CSS(GetWorld(), LogARBase_Messaging, Warning, TEXT("Cleanup on non empty message"));
		MessageQueue.clear();
	}

	return Super::CleanUp(for_destroy, close_reason);
}

bool UMessageChannel::CanStopTicking() const
{
	// TODO(cdc): Investigate is there is a way to make UChannel not tick (bPendingDormancy).
	return Super::CanStopTicking();
}

namespace
{
using namespace ar;

std::unique_ptr<Message> UnserializeMessage(FInBunch& bunch)
{
	// All messages start with their type, so we can use then to generate the correct message.
	FName type;
	bunch << type;

	if (bunch.IsError())
	{
		return nullptr;
	}

	// We create an instance of this message from the type and use virtual functions to call the
	// correct deserialization function.
	NotNullPtr entry = FindMessageTypeRegistryEntry(type);
	auto message = entry->FactoryFunction();
	check(message);
	message->Serialize(bunch);

	return message;
}

} // namespace

void UMessageChannel::ReceivedBunch(FInBunch& bunch)
{
	check(OwningEndpoint);

	// Check if the connection is closed.
	if (!Connection || Connection->GetConnectionState() == USOCK_Closed)
	{
		return;
	}

	while (!bunch.AtEnd())
	{
		auto message = UnserializeMessage(bunch);
		check(message);
		OwningEndpoint->ReceiveMessageFromChannel(this, std::move(message));
	}

	checkf(!bunch.IsError(), TEXT("Channel %s: Processing bunch"),
		   *OwningEndpoint->GetId().ToString());
}

namespace
{

bool WriteMessage(NotNullPtr<UMessageChannel> net_channel, std::unique_ptr<Message>&& message)
{
	FOutBunch bunch(net_channel, false);
	// TODO(cdc): Have reliable be part of the sending params.

	if (bunch.IsError())
	{
		return false;
	}

	// We write the message type so it can be decoded (See |ReceivedBunch|).
	// Then we serialize the rest, which is message dependent.
	FName type = message->GetMessageType();
	bunch << type;
	message->Serialize(bunch);
	if (bunch.IsError())
	{
		return false;
	}

	// If everything went well, we send the message over the wire.
	net_channel->SendBunch(&bunch, true);
	return true;
}

} // namespace

void UMessageChannel::Tick()
{
	// We call Super after because it does a check for dormancy which should be done after
	// processing all of our messages.
	ON_SCOPE_EXIT
	{
		Super::Tick();
	};

	// If we're not hooked to an channel, we don't care.
	if (!OwningEndpoint)
	{
		return;
	}

	while (!MessageQueue.empty())
	{
		if (Closing)
		{
			AR_LOG_CSS(GetWorld(), LogARBase_Messaging, Warning,
					   TEXT("Channel %s: Sending message when closing"),
					   *OwningEndpoint->GetId().ToString());
			return;
		}

		// If the connection is not ready, we retry next tick.
		if (!Connection->IsNetReady(kSaturate))
		{
			AR_LOG_CSS(GetWorld(), LogARBase_Messaging, Warning,
					   TEXT("Channel %s: Tick %d: Network saturated"),
					   *OwningEndpoint->GetId().ToString(), Connection->TickCount);
			return;
		}

		// Pop the message from the queue and attempt to send it over the wire.
		auto message = std::move(MessageQueue.front());
		check(message);
		MessageQueue.pop_front();
		if (!WriteMessage(this, std::move(message)))
		{
			AR_LOG_CSS(GetWorld(), LogARBase_Messaging, Error,
					   TEXT("Channel %s: writing message to channel error"),
					   *OwningEndpoint->GetId().ToString());
			return;
		}
	}
}