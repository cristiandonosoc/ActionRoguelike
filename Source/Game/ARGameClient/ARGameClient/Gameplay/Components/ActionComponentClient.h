#pragma once

#include <ARBase/ClientServerSplit.h>

class UARAction;
class UARActionComponent;

namespace ar
{
namespace client
{

class ARGAMECLIENT_API ActionComponentClient
{
	GENERATED_LEAF_CLIENT_SPLIT(UARActionComponent, ActionComponentClient);

public:
	void PredictStartActionByName(const FName& name, AActor* instigator);

	void StartActionByName(const FName& name, AActor* instigator);
	void StartAction(NotNullPtr<UARAction> action, AActor* instigator);
	void StopAction(NotNullPtr<UARAction> action, AActor* instigator);
};

} // namespace client
} // namespace ar



