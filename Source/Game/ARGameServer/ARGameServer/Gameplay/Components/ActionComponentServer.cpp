#include <ARGameServer/Gameplay/Components/ActionComponentServer.h>

#include <ARBase/Subsystems/ARStreamingSubsystem.h>
#include <ARGame/ARDebugCategories.h>
#include <ARGame/Gameplay/Actions/ARAction.h>
#include <ARGame/Gameplay/Components/ARActionComponent.h>

#include <Engine/ActorChannel.h>

namespace ar
{
namespace server
{

void ActionComponentServer::StartActionByName(const FName& name, AActor* instigator,
											  FPredictedStartActionContext&& context)
{
	NotNullPtr<UARAction> action = GetBase()->FindAction(name);
	StartAction(action, instigator, std::move(context));
}

void ActionComponentServer::StartAction(NotNullPtr<UARAction> action, AActor* instigator,
										FPredictedStartActionContext&& context)
{
	check(!action->GetIsRunning());

	GetBase()->ActiveGameplayTags.AppendTags(action->GetGrantsTags());
	action->ServerStart(instigator);

	// We also let the clients know that this action has started.
	GetBase()->RPC_Multicast_StartAction(action, instigator);
}

void ActionComponentServer::StopAction(NotNullPtr<UARAction> action, AActor* instigator)
{
	check(action->GetIsRunning());
	action->ServerStop(instigator);
	GetBase()->ActiveGameplayTags.RemoveTags(action->GetGrantsTags());

	// We also let the clients know that this action has stopped.
	GetBase()->RPC_Multicast_StopAction(action, instigator);
}

void ActionComponentServer::BeginPlay()
{
	// TODO(cdc): Do async loading.
	const auto& default_actions = GetBase()->GetServer_DefaultActions();

	NotNullPtr streamer = GetWorld()->GetGameInstance()->GetSubsystem<UARStreamingSubsystem>();
	auto paths = UARStreamingSubsystem::ExtractSoftObjectPaths(default_actions);
	streamer->RequestSyncLoad(paths);

	// Now that the classes are loaded, we can create the objects.
	for (const auto& action_soft_class : default_actions)
	{
		NotNullPtr<UClass> uclass = action_soft_class.Get();

		// We assume the action component holder is the instigator for these actions.
		GetBase()->AddAction(TSubclassOf<UARAction>(uclass), GetOwner());
	}
}

bool ActionComponentServer::ReplicateSubObjects(NotNullPtr<UActorChannel> channel,
												NotNullPtr<FOutBunch> bunch,
												NotNullPtr<FReplicationFlags> rep_flags)
{
	// Call the parent of the base to see if there was some replication there.
	using Super = UARActionComponent::Super;
	bool wrote_something = GetBase()->Super::ReplicateSubobjects(channel, bunch, rep_flags);

	for (const auto& action : GetBase()->GetActions())
	{
		check(action);
		wrote_something |= channel->ReplicateSubobject(action.Get(), *bunch, *rep_flags);
	}

	return wrote_something;
}


} // namespace server
} // namespace ar