// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTService.h"
#include "CoreMinimal.h"

#include "ARBTService_CheckAttackRange.generated.h"

/**
 *
 */
UCLASS()
class ARGAME_API UARBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()

protected:
	virtual void TickNode(UBehaviorTreeComponent& owner, uint8* node_memory, float delta) override;

protected:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector WithinAttackRangeKey;
};
