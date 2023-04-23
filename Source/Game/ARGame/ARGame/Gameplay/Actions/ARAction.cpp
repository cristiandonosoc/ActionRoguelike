#include <ARGame/Gameplay/Actions/ARAction.h>

#include <ARBase/Logging.h>
#include <ARBase/NotNullPtr.h>
#include <ARGame/ARDebugCategories.h>
#include <ARGame/Gameplay/Components/ARActionComponent.h>

UARActionComponent* UARAction::GetOwningComponent() const
{
	NotNullPtr oc = Cast<UARActionComponent>(GetOuter());
	return oc.Get();
}

UWorld* UARAction::GetWorld() const
{
	if (UARActionComponent* action_component = Cast<UARActionComponent>(GetOuter()))
	{
		return action_component->GetWorld();
	}

	return nullptr;
}

void UARAction::DispatchClientStopPrediction(AActor* instigator)
{
	CHECK_RUNNING_ON_CLIENT(GetOwningComponent());

	// We only dispatch a client stop prediction if we're running.
	// It could happen that the serer sends us a stop message before this prediction call.
	// In that case, we ignore.
	if (IsRunning)
	{
		ClientPredictStop(instigator);
	}
}

void UARAction::DispatchServerStop(AActor* instigator)
{
	CHECK_RUNNING_ON_SERVER(GetOwningComponent());

	// We make the roundabout via the ActionComponent to make sure the accounting and RPCs are
	// triggered properly.
	// This will call |ServerStop| on this action.
	GetOwningComponent()->GetServerSplit()->StopAction(this, instigator);
}

bool UARAction::CanStart_Implementation(AActor* instigator)
{
	auto& active_tags = GetOwningComponent()->GetActiveGameplayTags();
	if (active_tags.HasAny(BlockedTags))
	{
		return false;
	}

	if (IsRunning)
	{
		return false;
	}

	return true;
}
FPredictedStartActionContext UARAction::ClientPredictStart_Implementation(AActor* instigator)
{
	CHECK_RUNNING_ON_CLIENT(GetOwningComponent());
	check(!IsClientStartPredicting);
	check(!IsRunning);
	check(!IsClientStopPredicting);

	AR_LOG_CSS(GetWorld(), LogAR_Actions, Log, TEXT("Predicting Action Start: %s (Action: %s)"),
			   *ActionName.ToString(), *GetNameSafe(this));

	IsClientStartPredicting = true;

	return {};
}

void UARAction::FinalizeClientStartPrediction_Implementation()
{
	CHECK_RUNNING_ON_CLIENT(GetOwningComponent());
	check(IsClientStartPredicting);
	check(!IsRunning);
	check(!IsClientStopPredicting);

	AR_LOG_CSS(GetWorld(), LogAR_Actions, Log, TEXT("Finalizing start prediction: %s (Action: %s)"),
			   *ActionName.ToString(), *GetNameSafe(this));

	IsClientStartPredicting = false;
}

void UARAction::ClientPredictStop_Implementation(AActor* instigator)
{
	CHECK_RUNNING_ON_CLIENT(GetOwningComponent());

	// If we're not running, it could be that the server already arrived with the stop, so there is
	// no need to run stop prediction stop again.
	if (!IsRunning)
	{
		check(!IsClientStopPredicting) return;
	}

	check(!IsClientStartPredicting);
	check(!IsClientStopPredicting);

	AR_LOG_CSS(GetWorld(), LogAR_Actions, Log, TEXT("Predicting Action Stop: %s (Action: %s)"),
			   *ActionName.ToString(), *GetNameSafe(this));

	IsClientStopPredicting = true;
}

void UARAction::FinalizeClientStopPrediction_Implementation()
{
	CHECK_RUNNING_ON_CLIENT(GetOwningComponent());
	check(!IsClientStartPredicting);
	check(IsRunning);
	check(IsClientStopPredicting);

	AR_LOG_CSS(GetWorld(), LogAR_Actions, Log, TEXT("Finalizing stop prediction: %s (Action: %s)"),
			   *ActionName.ToString(), *GetNameSafe(this));

	IsClientStopPredicting = false;
}

void UARAction::ClientStart_Implementation(AActor* instigator)
{
	CHECK_RUNNING_ON_CLIENT(GetOwningComponent());
	check(!IsRunning);
	check(!IsClientStopPredicting);

	// If we're predicting that the action started (is running), we activate any cleanup it might
	// want to do before starting the action "for real" client-wise.
	if (IsClientStartPredicting)
	{
		FinalizeClientStartPrediction();
	}

	check(!IsClientStartPredicting);

	AR_LOG_CSS(GetWorld(), LogAR_Actions, Log, TEXT("Action Start: %s (Action: %s)"),
			   *ActionName.ToString(), *GetNameSafe(this));

	IsRunning = true;
	// TODO(cdc): Should this be done via replication?
	// GetOwningComponent()->GetActiveGameplayTags().AppendTags(GrantsTags);
}

void UARAction::ClientStop_Implementation(AActor* instigator)
{
	CHECK_RUNNING_ON_CLIENT(GetOwningComponent());
	check(IsRunning);

	// If we're predicting that the action stopped, we activate any cleanup that it might want to do
	// before stopping the action "for real" client-wise.
	if (IsClientStopPredicting)
	{
		FinalizeClientStopPrediction();
	}

	AR_LOG_CSS(GetWorld(), LogAR_Actions, Log, TEXT("Action Stop: %s (Action: %s)"),
			   *ActionName.ToString(), *GetNameSafe(this));

	// TODO(cdc): Should this be done via replication?
	// GetOwningComponent()->GetActiveGameplayTags().RemoveTags(GrantsTags);
	IsRunning = false;
}

void UARAction::ServerStart_Implementation(AActor* instigator,
										   const FPredictedStartActionContext& context)
{
	CHECK_RUNNING_ON_SERVER(GetOwningComponent());
	check(!IsClientStartPredicting);
	check(!IsClientStopPredicting);

	check(!IsRunning);

	AR_LOG_CSS(GetWorld(), LogAR_Actions, Log, TEXT("Action Start: %s (Action: %s)"),
			   *ActionName.ToString(), *GetNameSafe(this));

	IsRunning = true;
}

void UARAction::ServerStop_Implementation(AActor* instigator)
{
	CHECK_RUNNING_ON_SERVER(GetOwningComponent());
	check(!IsClientStartPredicting);
	check(!IsClientStopPredicting);

	check(IsRunning);

	AR_LOG_CSS(GetWorld(), LogAR_Actions, Log, TEXT("Action Stop: %s (Action: %s)"),
			   *ActionName.ToString(), *GetNameSafe(this));

	IsRunning = false;
}