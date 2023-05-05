#pragma once

#include <ARBase/Messaging/Message.h>

#include <deque>

#include "NetMessageChannel.generated.h"

// UNetMessageChannel is our custom UChannel implementation that understands our messages and is
// capable to serialize them and send them over the wire.
UCLASS()
class UNetMessageChannel : public UChannel
{
	GENERATED_BODY()
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
	std::deque<std::unique_ptr<ar::Message>> MessageQueue;
};
