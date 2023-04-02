#include <ARBase/Blueprints/BlueprintUtils.h>

#include <ARBase/ClientServerSplit.h>

float UBlueprintUtils::GetAccurateTime()
{
	return FApp::GetCurrentTime() - GStartTime;
}

bool UBlueprintUtils::ActorRunningInClient(const AActor* actor)
{
	return ARClientServerGlobals::RunningInClient(actor);
}

bool UBlueprintUtils::ActorComponentRunningInClient(const UActorComponent* actor_component)
{
	return ARClientServerGlobals::RunningInClient(actor_component);
}

bool UBlueprintUtils::ActorRunningInServer(const AActor* actor)
{
	return ARClientServerGlobals::RunningInServer(actor);
}

bool UBlueprintUtils::ActorComponentRunningInServer(const UActorComponent* actor_component)
{
	return ARClientServerGlobals::RunningInServer(actor_component);
}