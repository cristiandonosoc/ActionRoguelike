// Fill out your copyright notice in the Description page of Project Settings.

#include <ARGame/AI/BehaviorTree/ARBTTask_RangeAttack.h>

#include <ARGame/AI/ARAICharacter.h>

#include <AIController.h>
#include <ARBase/NotNullPtr.h>
#include <BehaviorTree/BlackboardComponent.h>

EBTNodeResult::Type UARBTTask_RangeAttack::ExecuteTask(UBehaviorTreeComponent& owner,
													   uint8* node_memory)
{
	// Get the target actor from the blackboard.
	UBlackboardComponent* blackboard = owner.GetBlackboardComponent();
	if (!ensure(blackboard))
	{
		return EBTNodeResult::Failed;
	}

	AActor* target_actor =
		Cast<AActor>(blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!target_actor)
	{
		return EBTNodeResult::Failed;
	}

	NotNullPtr<AAIController> ai = owner.GetAIOwner();
	auto* character = Cast<AARAICharacter>(ai->GetPawn());
	if (!character)
	{
		return EBTNodeResult::Failed;
	}

	if (!character->PerformPrimaryAttack(*target_actor))
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}