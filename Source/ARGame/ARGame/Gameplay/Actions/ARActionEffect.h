#pragma once

#include <ARGame/Gameplay/Actions/ARAction.h>
#include <CoreMinimal.h>

#include "ARActionEffect.generated.h"

UCLASS()
class ARGAME_API UARActionEffect : public UARAction
{
	GENERATED_BODY()

public:
	UARActionEffect();

protected:
	// INTERFACE_BEGIN(UARAction)
	virtual void Start_Implementation(AActor* instigator) override;
	virtual void Stop_Implementation(AActor* instigator) override;
	// INTERFACE_END(UARAction)

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void Execute(AActor* instigator);

	// INTERFACE_BEGIN(UARActionEffect)
	void Execute_Implementation(AActor* instigator) {}
	// INTERFACE_END(UARActionEffect)

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period;

protected:
	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;
};