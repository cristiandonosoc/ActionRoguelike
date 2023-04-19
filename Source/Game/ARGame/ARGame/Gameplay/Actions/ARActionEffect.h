#pragma once

#include <ARGame/Gameplay/Actions/ARAction.h>
#include <CoreMinimal.h>

#include "ARActionEffect.generated.h"

struct FPredictedStartActionContext;

UCLASS()
class ARGAME_API UARActionEffect : public UARAction
{
	GENERATED_BODY()

public:
	UARActionEffect();

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void Execute(AActor* instigator);

protected:
	// INTERFACE_BEGIN(UARAction)
	virtual void ServerStart_Implementation(AActor* instigator,
											const FPredictedStartActionContext& context) override;
	virtual void ServerStop_Implementation(AActor* instigator) override;
	// INTERFACE_END(UARAction)

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