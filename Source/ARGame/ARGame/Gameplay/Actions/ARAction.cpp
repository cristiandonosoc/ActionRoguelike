#include <ARGame/Gameplay/Actions/ARAction.h>
#include <ARGame/Gameplay/Components/ARActionComponent.h>
#include <Misc/MTAccessDetector.h>

void UARAction::Start_Implementation(AActor* instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Action Start: %s (Actor: %s)"), *ActionName.ToString(),
		   *GetNameSafe(this));
}

void UARAction::Stop_Implementation(AActor* instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Action Stop: %s (Actor: %s)"), *ActionName.ToString(),
		   *GetNameSafe(this));
}

UWorld* UARAction::GetWorld() const
{
	if (UARActionComponent* action_component = Cast<UARActionComponent>(GetOuter()))
	{
		return action_component->GetWorld();
	}

	return nullptr;
}