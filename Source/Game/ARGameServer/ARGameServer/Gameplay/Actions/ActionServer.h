#pragma once

#include <ARBase/ClientServerSplit.h>
#include <ARGame/Gameplay/Components/ARActionComponentData.h>

class UARAction;

namespace ar
{
namespace server
{

class ARGAMESERVER_API ActionServer
{
	GENERATED_LEAF_SERVER_SPLIT(UARAction, ActionServer);

public:
	// Command is something that is queued to happen within the action component.
	struct Command
	{
		enum class CommandKind : uint8
		{
			StartAction,
			StopAction,
			Invalid,
		};

		CommandKind Kind = CommandKind::Invalid;
		AActor* Instigator = nullptr;
		FPredictedStartActionContext Context = {};
	};

public:
	void Start(AActor* instigator, FPredictedStartActionContext&& context);
	void Stop(AActor* instigator);

private:
	void BeginPlay();

	// ProcessCommandQueue will go over all the queue commands and process them one by one.
	// This can be called many times and only the first time will apply.
	// This permits us to queue many commands and have all of them try to prompt the queue to
	// process, but only the first one will work.
	void ProcessCommandQueue();

	void ProcessStartCommand(Command&& command);
	void ProcessStopCommand(Command&& command);

private:
	TArray<Command> CommandQueue;
	bool CurrentlyProcessing = false;
};



} // namespace server
} // namespace ar