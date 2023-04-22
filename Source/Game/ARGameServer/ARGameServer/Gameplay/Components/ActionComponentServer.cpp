#include <ARGameServer/Gameplay/Components/ActionComponentServer.h>

#include <ARBase/Subsystems/ARStreamingSubsystem.h>
#include <ARGame/ARDebugCategories.h>
#include <ARGame/Gameplay/Actions/ARAction.h>
#include <ARGame/Gameplay/Components/ARActionComponent.h>

#include <Engine/ActorChannel.h>
#include <Misc/TypeContainer.h>

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
	// We push the command because the component could be in the middle of processing a command, so
	// we need to wait until that one is processed.
	ActionCommand command = {};
	command.Action = action.Get();
	command.Kind = ActionCommand::CommandKind::StartAction;
	command.Instigator = instigator;
	command.Context = std::move(context);
	CommandQueue.Push(std::move(command));

	// We prompt the component to process the queue, if it isn't already.
	ProcessCommandQueue();
}

void ActionComponentServer::StopAction(NotNullPtr<UARAction> action, AActor* instigator)
{
	// We queue a stop command
	ActionCommand command = {};
	command.Action = action.Get();
	command.Kind = ActionCommand::CommandKind::StopAction;
	command.Instigator = instigator;
	CommandQueue.Push(std::move(command));

	// We prompt the component to process the queue, if it isn't already.
	ProcessCommandQueue();
}

void ActionComponentServer::BeginPlay()
{
	CommandQueue.Reserve(4);

	// TODO(cdc): Do async loading.
	const auto& default_actions = GetBase()->Server_DefaultActions;

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

void ActionComponentServer::ProcessCommandQueue()
{
	// If we're already processing, we don't need to re-enter.
	if (CurrentlyProcessing)
	{
		return;
	}

	// We create a guard to mark this queue under being process, and we clear it at the end of the
	// scope.
	CurrentlyProcessing = true;
	ON_SCOPE_EXIT
	{
		CurrentlyProcessing = false;
	};

	// We run until the command queue is empty.
	check(!CommandQueue.IsEmpty());
	do
	{
		// We pop one command at a time and run it.
		// This could add new commands to the queue.
		ActionCommand command = CommandQueue.Pop();
		check(command.Action);

		switch (command.Kind)
		{
		case ActionCommand::CommandKind::StartAction:
			ProcessStartActionCommand(std::move(command));
			break;
		case ActionCommand::CommandKind::StopAction:
			ProcessStopActionCommand(std::move(command));
			break;
		default:
			checkf(false, TEXT("Invalid command kind"));
		}
	}
	while (!CommandQueue.IsEmpty());
}
void ActionComponentServer::ProcessStartActionCommand(ActionCommand&& command)
{
	check(!command.Action->GetIsRunning());

	GetBase()->ActiveGameplayTags.AppendTags(command.Action->GetGrantsTags());
	command.Action->ServerStart(command.Instigator, std::move(command.Context));

	// We also let the clients know that this action has started.
	GetBase()->RPC_Multicast_StartAction(command.Action, command.Instigator);
}

void ActionComponentServer::ProcessStopActionCommand(ActionCommand&& command)
{
	check(command.Action->GetIsRunning());
	command.Action->ServerStop(command.Instigator);
	GetBase()->ActiveGameplayTags.RemoveTags(command.Action->GetGrantsTags());

	// We also let the clients know that this action has stopped.
	GetBase()->RPC_Multicast_StopAction(command.Action, command.Instigator);
}

} // namespace server
} // namespace ar