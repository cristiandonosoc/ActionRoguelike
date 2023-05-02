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

private:
	void BeginPlay();
	bool ReplicateSubObjects(NotNullPtr<UActorChannel> channel, NotNullPtr<FOutBunch> bunch,
							 NotNullPtr<FReplicationFlags> rep_flags);
};

} // namespace server
} // namespace ar