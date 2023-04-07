#include <ARGame/Gameplay/Actions/ARAction.h>

#include <ARBase/NotNullPtr.h>
#include <ARGame/Gameplay/Components/ARActionComponent.h>
#include <Misc/MTAccessDetector.h>

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

bool UARAction::CanStart_Implementation(AActor* instigator)
{
	auto& active_tags = GetOwningComponent()->GetActiveGameplayTags();
	if (active_tags.HasAny(BlockedTags))
	{
		return false;
	}

	if (IsRunning && !CanBeStartedMultipleTimes)
	{
		return false;
	}

	return true;
}

void UARAction::ClientPredictStart_Implementation(AActor* instigator)
{
	CHECK_RUNNING_ON_CLIENT(GetOwningComponent());
	check(!IsClientPredicting);
	check(!IsRunning);

	UE_LOG(LogTemp, Log, TEXT("Client: Predicting Action Start: %s (Actor: %s)"),
		   *ActionName.ToString(), *GetNameSafe(this));

	IsClientPredicting = true;
}

void UARAction::ClientPredictStop_Implementation(AActor* instigator)
{
	CHECK_RUNNING_ON_CLIENT(GetOwningComponent());
	check(IsClientPredicting);
	check(!IsRunning);

	UE_LOG(LogTemp, Log, TEXT("Client: Predicting Action Stop: %s (Actor: %s)"),
		   *ActionName.ToString(), *GetNameSafe(this));

	IsClientPredicting = false;
}

void UARAction::ClientStart_Implementation(AActor* instigator)
{
	CHECK_RUNNING_ON_CLIENT(GetOwningComponent());
	check(!IsRunning);

	// If we're client predicting, we activate any cleanup it might want to do before starting the
	// action client-wise.
	if (IsClientPredicting)
	{
		ClientPredictStop_Implementation(instigator);
	}

	check(!IsClientPredicting);
	
	UE_LOG(LogTemp, Log, TEXT("Client: Action Start: %s (Actor: %s)"), *ActionName.ToString(),
		   *GetNameSafe(this));

	IsRunning = true;
	// TODO(cdc): Should this be done via replication?
	GetOwningComponent()->GetActiveGameplayTags().AppendTags(GrantsTags);
}

void UARAction::ClientStop_Implementation(AActor* instigator)
{
	CHECK_RUNNING_ON_CLIENT(GetOwningComponent());
	check(!IsClientPredicting);
	check(IsRunning);
	
	UE_LOG(LogTemp, Log, TEXT("Client: Action Stop: %s (Actor: %s)"), *ActionName.ToString(),
		   *GetNameSafe(this));
	
	// TODO(cdc): Should this be done via replication?
	GetOwningComponent()->GetActiveGameplayTags().RemoveTags(GrantsTags);
	IsRunning = false;
}

void UARAction::ServerStart_Implementation(AActor* instigator)
{
	CHECK_RUNNING_ON_SERVER(GetOwningComponent());
	check(!IsRunning);
	check(!IsClientPredicting);
	
	UE_LOG(LogTemp, Log, TEXT("Server: Action Start: %s (Actor: %s)"), *ActionName.ToString(),
		   *GetNameSafe(this));

	IsRunning = true;
	GetOwningComponent()->GetActiveGameplayTags().AppendTags(GrantsTags);
}

void UARAction::ServerStop_Implementation(AActor* instigator)
{
	CHECK_RUNNING_ON_SERVER(GetOwningComponent());
	check(IsRunning);
	
	UE_LOG(LogTemp, Log, TEXT("Server: Action Stop: %s (Actor: %s)"), *ActionName.ToString(),
		   *GetNameSafe(this));
	
	GetOwningComponent()->GetActiveGameplayTags().RemoveTags(GrantsTags);
	IsRunning = false;
}