// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "CoreMinimal.h"

#include "ARBTTask_RangeAttack.generated.h"

UCLASS()
class ARGAME_API UARBTTask_RangeAttack : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner,
											uint8* node_memory) override;

protected:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetActorKey;
};