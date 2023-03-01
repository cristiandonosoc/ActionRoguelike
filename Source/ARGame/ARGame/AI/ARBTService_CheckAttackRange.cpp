// Fill out your copyright notice in the Description page of Project Settings.

#include "ARBTService_CheckAttackRange.h"

#include "AIController.h"
#include "ARBase/NotNullPtr.h"
#include "BehaviorTree/BlackboardComponent.h"

void UARBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& owner, uint8* node_memory,
											 float delta)
{
	Super::TickNode(owner, node_memory, delta);

	// Check distance between AI awn and target actor.
	UBlackboardComponent* blackboard = owner.GetBlackboardComponent();
	if (!ensure(blackboard))
	{
		return;
	}

	AActor* target_actor = Cast<AActor>(blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!target_actor)
	{
		return;
	}

	NotNullPtr<AAIController> ai_controller = owner.GetAIOwner();
	NotNullPtr<APawn> ai_pawn = ai_controller->GetPawn();

	float dist = FVector::Distance(target_actor->GetActorLocation(), ai_pawn->GetActorLocation());

	bool within_range = dist < AttackRange;

	// We only check for line of sight if we're in range. No need to check otherwise.
	if (within_range)
	{
		within_range = ai_controller->LineOfSightTo(target_actor);
	}

	blackboard->SetValueAsBool(WithinAttackRangeKey.SelectedKeyName, within_range);
}
