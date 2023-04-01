#pragma once

#include <ARBase/ClientServerSplit.h>

class UARActorAttachedWidget;
class UARInteractionComponent;

class ARGAMECLIENT_API ARInteractionComponentClient
{
	GENERATED_CLIENT_SPLIT(UARInteractionComponent, ARInteractionComponentClient);

public:
	static constexpr float kFocusCheckPeriod = 0.5f;

public:
	void BeginPlay();
	void EndPlay();
	
	void NotifyIsLocalControlled();
	void FindBestInteractable();


private:
	FTimerHandle FindFocusTimerHandle;
};