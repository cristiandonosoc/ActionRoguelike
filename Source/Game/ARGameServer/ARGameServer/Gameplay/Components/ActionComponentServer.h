#pragma once

#include <ARBase/ClientServerSplit.h>

class UARAction;
class UARActionComponent;

namespace ar
{
namespace server
{

class ARGAMESERVER_API ActionComponentServer
{
	GENERATED_LEAF_SERVER_SPLIT(UARActionComponent, ActionComponentServer);

public:

	void BeginPlay();
	bool ReplicateSubObjects(NotNullPtr<UActorChannel> channel, NotNullPtr<FOutBunch> bunch,
							 NotNullPtr<FReplicationFlags> rep_flags);
	
	void StartAction(NotNullPtr<UARAction> action, AActor* instigator);
	void StopAction(NotNullPtr<UARAction> action, AActor* instigator);
};

} // namespace server
} // namespace ar