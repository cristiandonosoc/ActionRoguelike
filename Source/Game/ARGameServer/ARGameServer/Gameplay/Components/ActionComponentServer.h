#pragma once

#include <ARBase/ClientServerSplit.h>

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
	void StartActionByName(const FName& name, AActor* instigator,
						   FPredictedStartActionContext&& context);
	void StartAction(NotNullPtr<UARAction> action, AActor* instigator,
					 FPredictedStartActionContext&& context);
	void StopAction(NotNullPtr<UARAction> action, AActor* instigator);

private:
	void BeginPlay();
	bool ReplicateSubObjects(NotNullPtr<UActorChannel> channel, NotNullPtr<FOutBunch> bunch,
							 NotNullPtr<FReplicationFlags> rep_flags);
};

} // namespace server
} // namespace ar