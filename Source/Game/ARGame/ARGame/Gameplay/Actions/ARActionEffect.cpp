#include <ARGame/Gameplay/Actions/ARActionEffect.h>

#include <ARGame/Gameplay/Components/ARActionComponent.h>

UARActionEffect::UARActionEffect()
{
	// Effects auto start by default.
	AutoStarts = true;
}

void UARActionEffect::OnServerStart_Implementation(AActor* instigator,
												   const FPredictedStartActionContext& context)
{
	Super::OnServerStart_Implementation(instigator, context);

	if (Duration > 0.0f)
	{
		FTimerDelegate delegate;
		delegate.BindLambda(
			[this, instigator, weakGuard = TWeakObjectPtr<UObject>(this)]()
			{
				if (weakGuard.IsValid())
				{
					OnServerStop(instigator);
				}
			});
		GetWorld()->GetTimerManager().SetTimer(DurationHandle, delegate, Duration, false);
	}

	if (Period > 0.0f)
	{
		FTimerDelegate delegate;
		delegate.BindLambda(
			[this, instigator, weakGuard = TWeakObjectPtr<UObject>(this)]()
			{
				if (weakGuard.IsValid())
				{
					Execute(instigator);
				}
			});
		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, delegate, Period, true);
	}
}

void UARActionEffect::OnServerStop_Implementation(AActor* instigator)
{
	CHECK_RUNNING_ON_SERVER(GetOwningComponent());
	auto& timer = GetWorld()->GetTimerManager();

	// We check if we had a period pending (could happen if the period and the duration would end
	// very close to each other. If it is, we execute it one last time.
	if (timer.GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		Execute(instigator);
	}

	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);

	GetOwningComponent()->RemoveAction(ActionName);

	Super::OnServerStop_Implementation(instigator);
}