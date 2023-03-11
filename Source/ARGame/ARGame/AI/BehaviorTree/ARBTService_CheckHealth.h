#pragma once

#include <BehaviorTree/BTService.h>
#include <CoreMinimal.h>

#include "ARBTService_CheckHealth.generated.h"

UCLASS()
class ARGAME_API UARBTService_CheckHealth : public UBTService
{
	GENERATED_BODY()

protected:
	virtual void TickNode(UBehaviorTreeComponent& owner, uint8* node_memory, float delta) override;

protected:
	UPROPERTY(EditAnywhere, meta=(ClampMin=0.1, ClampMax=0.8))
	float LowHealthThreshold = 0.4;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector LowHealthKey;
};