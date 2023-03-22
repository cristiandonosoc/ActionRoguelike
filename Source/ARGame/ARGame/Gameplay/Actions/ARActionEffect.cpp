#include <ARGame/Gameplay/Actions/ARActionEffect.h>
#include <ARGame/Gameplay/Components/ARActionComponent.h>

void UARActionEffect::Start_Implementation(AActor* instigator)
{
	Super::Start_Implementation(instigator);

	if (Duration > 0.0f)
	{
		FTimerDelegate delegate;
		delegate.BindUFunction(this, "Stop", instigator);
		GetWorld()->GetTimerManager().SetTimer(DurationHandle, delegate, Duration, false);
	}

	if (Period > 0.0f)
	{
		FTimerDelegate delegate;
		delegate.BindUFunction(this, "Execute", instigator);
		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, delegate, Period, true);
	}
}

void UARActionEffect::Stop_Implementation(AActor* instigator)
{
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);

	GetOwningComponent().RemoveAction(ActionName);
	
	Super::Stop_Implementation(instigator);
}