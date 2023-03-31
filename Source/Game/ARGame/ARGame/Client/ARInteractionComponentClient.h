#pragma once
#include <ARBase/ClientServerSplit.h>
#ifdef AR_BUILD_CLIENT

class UARInteractionComponent;

class ARInteractionComponentClient
{
	GENERATED_CLIENT_SPLIT(UARInteractionComponent, ARInteractionComponentClient);

public:
	static constexpr float kFocusCheckPeriod = 0.5f;

public:
	void NotifyIsLocalControlled();
	void FindBestInteractable();

private:
	void EndPlay();

private:
	FTimerHandle FindFocusTimerHandle;
};

#endif // AR_BUILD_CLIENT