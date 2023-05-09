#pragma once

#include "ARPlayerController.generated.h"

UCLASS()
class AARPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// ReceivedPlayer is called when a net connection is associated with this player controller.
	// This is useful to hook it to other systems that depend on the net connection (liek the
	// messaging system).
	virtual void ReceivedPlayer() override;
};
