#include <ARGame/Gameplay/Actions/ARAction.h>

#include <ARBase/NotNullPtr.h>
#include <ARGame/Gameplay/Components/ARActionComponent.h>
#include <Misc/MTAccessDetector.h>

bool UARAction::CanStart_Implementation(AActor* instigator)
{
	auto& active_tags = GetOwningComponent().GetActiveGameplayTags();
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

void UARAction::Start_Implementation(AActor* instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Action Start: %s (Actor: %s)"), *ActionName.ToString(),
		   *GetNameSafe(this));

	IsRunning = true;
	GetOwningComponent().GetActiveGameplayTags().AppendTags(GrantsTags);
}

void UARAction::Stop_Implementation(AActor* instigator)
{
	check(IsRunning);
	UE_LOG(LogTemp, Log, TEXT("Action Stop: %s (Actor: %s)"), *ActionName.ToString(),
		   *GetNameSafe(this));

	GetOwningComponent().GetActiveGameplayTags().RemoveTags(GrantsTags);
	IsRunning = false;
}

UARActionComponent& UARAction::GetOwningComponent()
{
	NotNullPtr oc = Cast<UARActionComponent>(GetOuter());
	return *oc;
}

UWorld* UARAction::GetWorld() const
{
	if (UARActionComponent* action_component = Cast<UARActionComponent>(GetOuter()))
	{
		return action_component->GetWorld();
	}

	return nullptr;
}