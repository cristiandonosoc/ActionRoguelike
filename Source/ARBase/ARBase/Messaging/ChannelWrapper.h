#pragma once

#include <ARBase/Messaging/Message.h>
#include <ARBase/Messaging/MessageChannel.h>

#include <deque>

#include "ChannelWrapper.generated.h"

// UChannelWrapper is our custom UChannel implementation that understands our messages and is
// capable to serialize them and send them over the wire.
UCLASS()
class UChannelWrapper : public UChannel
{
	GENERATED_BODY()

public:
	void SetOwningChannel(ar::MessageChannel* channel) { OwningChannel = channel; }

public:
	void Enqueue(std::unique_ptr<ar::Message>&& message);

public:
	// INTERFACE_BEGIN(UChannel)
	void Init(UNetConnection* in_connection, int32 in_ch_index,
			  EChannelCreateFlags create_flags) override;
	virtual bool CleanUp(const bool for_destroy, EChannelCloseReason close_reason) override;
	virtual bool CanStopTicking() const override;
	virtual void ReceivedBunch(FInBunch& bunch) override;
	virtual void Tick() override;
	// INTERFACE_END(UChannel)

private:
	static constexpr bool kSaturate = false;

private:
	ar::MessageChannel* OwningChannel; // Not owning.
	std::deque<std::unique_ptr<ar::Message>> MessageQueue;
};
