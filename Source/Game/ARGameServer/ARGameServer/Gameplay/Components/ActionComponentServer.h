#pragma once

#include <ARBase/ClientServerSplit.h>
#include <ARGame/Gameplay/Components/ARActionComponentData.h>

class UARAction;
class UARActionComponent;
struct FPredictedStartActionContext;

namespace ar
{
namespace server
{

class ARGAMESERVER_API ActionComponentServer
{
	GENERATED_LEAF_SERVER_SPLIT(UARActionComponent, ActionComponentServer);

public:
	// ActionCommand is something that is queued to happen within the action component.
	struct ActionCommand
	{
		enum class CommandKind : uint8
		{
			StartAction,
			StopAction,
			Invalid,
		};

		UARAction* Action = nullptr;
		CommandKind Kind = CommandKind::Invalid;
		AActor* Instigator = nullptr;
		FPredictedStartActionContext Context = {};
	};

public:
	void AddAction(TSubclassOf<UARAction> action_class, AActor* instigator);
	
	void StartActionByName(const FName& name, AActor* instigator,
						   FPredictedStartActionContext&& context);
	void StartAction(NotNullPtr<UARAction> action, AActor* instigator,
					 FPredictedStartActionContext&& context);
	void StopAction(NotNullPtr<UARAction> action, AActor* instigator);

private:
	void BeginPlay();
	bool ReplicateSubObjects(NotNullPtr<UActorChannel> channel, NotNullPtr<FOutBunch> bunch,
							 NotNullPtr<FReplicationFlags> rep_flags);

	// ProcessCommandQueue will go over all the queue commands and process them one by one.
	// This can be called many times and only the first time will apply.
	// This permits us to queue many commands and have all of them try to prompt the queue to
	// process, but only the first one will work.
	void ProcessCommandQueue();

	void ProcessStartActionCommand(ActionCommand&& command);
	void ProcessStopActionCommand(ActionCommand&& command);


private:
	TArray<ActionCommand> CommandQueue;
	bool CurrentlyProcessing = false;
};

} // namespace server
} // namespace ar