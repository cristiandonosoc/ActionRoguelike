#include <ARGameClient/Gameplay/Components/ActionComponentClient.h>

#include <ARBase/Logging.h>
#include <ARGame/ARDebugCategories.h>
#include <ARGame/Gameplay/Actions/ARAction.h>
#include <ARGame/Gameplay/Components/ARActionComponent.h>

namespace ar
{
namespace client
{

namespace
{

UARAction* FindStartableAction(NotNullPtr<UARActionComponent> action_component, const FName& name,
							   AActor* instigator)
{
	UARAction* action = action_component->FindAction(name);
	if (!action)
	{
		FString msg = FString::Printf(TEXT("No action for name %s found"), *name.ToString());
		debug::DrawText(ar::ACTIONS, action_component->GetWorld(), msg, FColor::Red, 2);
		return nullptr;
	}

	if (action->GetIsClientStartPredicting())
	{
		FString msg =
			FString::Printf(TEXT("Action %s is client predicting already"), *name.ToString());
		debug::DrawText(ar::ACTIONS, action_component->GetWorld(), msg, FColor::Red, 2);
		return nullptr;
	}

	if (!action->CanStart(instigator))
	{
		FString msg = FString::Printf(TEXT("Action %s cannot start"), *name.ToString());
		debug::DrawText(ar::ACTIONS, action_component->GetWorld(), msg, FColor::Red, 2);
		return nullptr;
	}

	return action;
}

} // namespace

void ActionComponentClient::PredictStartActionByName(const FName& name, AActor* instigator)
{
	UARAction* action = FindStartableAction(GetBase(), name, instigator);
	if (!action)
	{
		AR_LOG_CSS(GetWorld(), LogAR_Actions, Error, TEXT("Action %s not found!"),
				   *name.ToString());
		return;
	}

	// If it's a client only action, there is no prediction needed so we go direct to start.
	if (action->GetIsClientOnly())
	{
		action->ClientStart(instigator);
		return;
	}

	// This is a networked ability, so we start the client prediction and then send a request for
	// the server to start the ability on its end.
	// TODO(cdc): Figure out a way of sending this without needing to go through the component.
	FPredictedStartActionContext context = action->ClientPredictStart(instigator);
	action->ServerStart(instigator, std::move(context));
}

void ActionComponentClient::StartActionByName(const FName& name, AActor* instigator)
{
	UARAction* action = FindStartableAction(GetBase(), name, instigator);
	if (!action)
	{
		AR_LOG_CSS(GetWorld(), LogAR_Actions, Error, TEXT("Action %s not found!"),
				   *name.ToString());
		return;
	}

	action->ClientStart(instigator);
}

} // namespace client
} // namespace ar