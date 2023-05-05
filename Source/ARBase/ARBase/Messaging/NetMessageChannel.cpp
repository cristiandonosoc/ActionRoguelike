#include <ARBase/Messaging/NetMessageChannel.h>

#include <ARBase/DebugTools.h>
#include <ARBase/Logging.h>
#include <ARBase/Messaging/Message.h>

void UNetMessageChannel::Enqueue(std::unique_ptr<ar::Message>&& message)
{
	check(message);
	checkf(!Closing, TEXT("Channel %s, MessageType %s: Enqueing to channel while closing"),
		   *GetName(), *message->GetMessageType().Id.ToString());
	MessageQueue.push_back(std::move(message));
}

void UNetMessageChannel::Init(UNetConnection* in_connection, int32 in_ch_index,
							  EChannelCreateFlags create_flags)
{
	Super::Init(in_connection, in_ch_index, create_flags);
	AR_LOG_CSS(GetWorld(), LogARBase_Messaging, Log, TEXT("Initializing channel %s"), *GetName());
}

bool UNetMessageChannel::CleanUp(const bool for_destroy, EChannelCloseReason close_reason)
{
	checkf(false, TEXT("TODO: IMPLEMENT ME"));
	return Super::CleanUp(for_destroy, close_reason);
}

bool UNetMessageChannel::CanStopTicking() const
{
	checkf(false, TEXT("TODO: IMPLEMENT ME"));
	return Super::CanStopTicking();
}

void UNetMessageChannel::ReceivedBunch(FInBunch& bunch)
{
	checkf(false, TEXT("TODO: IMPLEMENT ME"));
	Super::ReceivedBunch(bunch);
}

void UNetMessageChannel::Tick()
{
	checkf(false, TEXT("TODO: IMPLEMENT ME"));
	Super::Tick();
}
