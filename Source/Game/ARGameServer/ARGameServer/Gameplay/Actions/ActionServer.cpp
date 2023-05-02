#include <ARGameServer/Gameplay/Actions/ActionServer.h>

#include <ARGame/Gameplay/Actions/ARAction.h>

namespace ar
{
namespace server
{

void ActionServer::Start(AActor* instigator, FPredictedStartActionContext&& context)
{
	// We push the command because the component could be in the middle of processing a command, so
	// we need to wait until that one is processed.
	Command command = {};
	command.Kind = Command::CommandKind::StartAction;
	command.Instigator = instigator;
	command.Context = std::move(context);
	CommandQueue.Push(std::move(command));

	// We prompt the component to process the queue, if it isn't already.
	ProcessCommandQueue();
}

void ActionServer::Stop(AActor* instigator)
{
	// We queue a stop command
	Command command = {};
	command.Kind = Command::CommandKind::StopAction;
	command.Instigator = instigator;
	CommandQueue.Push(std::move(command));

	// We prompt the component to process the queue, if it isn't already.
	ProcessCommandQueue();
}

void ActionServer::BeginPlay()
{
	CommandQueue.Reserve(4);
}

void ActionServer::ProcessCommandQueue()
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
		Command command = CommandQueue.Pop();

		switch (command.Kind)
		{
		case Command::CommandKind::StartAction:
			ProcessStartCommand(std::move(command));
			break;
		case Command::CommandKind::StopAction:
			ProcessStopCommand(std::move(command));
			break;
		default:
			checkf(false, TEXT("Invalid command kind"));
		}
	}
	while (!CommandQueue.IsEmpty());
}

void ActionServer::ProcessStartCommand(Command&& command)
{
	check(!GetBase()->IsRunning);

	// TODO(cdc): This should be done in the action component.
	GetBase()->GetOwningComponent()->GetActiveGameplayTags().AppendTags(GetBase()->GetGrantsTags());
	GetBase()->ServerStart(command.Instigator, std::move(command.Context));

	// We also let the clients know that this action has started.
	GetBase()->RPC_Multicast_ClientStart(command.Instigator);
}

void ActionServer::ProcessStopCommand(Command&& command)
{
	check(GetBase()->IsRunning);
	GetBase()->ServerStop(command.Instigator);
	// TODO(cdc): This should be done in the action component.
	GetBase()->GetOwningComponent()->GetActiveGameplayTags().RemoveTags(GetBase()->GetGrantsTags());

	// We also let the clients know that this action has stopped.
	GetBase()->RPC_Multicast_ClientStop(command.Instigator);
}


} // namespace server
} // namespace ar