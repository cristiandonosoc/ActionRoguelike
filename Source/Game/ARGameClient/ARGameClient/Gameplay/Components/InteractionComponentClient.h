#pragma once

#include <ARBase/ClientServerSplit.h>

class UARActorAttachedWidget;
class UARInteractionComponent;

namespace ar
{
namespace client
{

class ARGAMECLIENT_API InteractionComponentClient
{
	GENERATED_CLIENT_SPLIT(UARInteractionComponent);

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

} // namespace client
} // namespace ar