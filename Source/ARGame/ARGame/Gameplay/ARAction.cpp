#include <ARGame/Gameplay/ARAction.h>

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