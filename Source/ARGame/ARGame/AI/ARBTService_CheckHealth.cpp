// Fill out your copyright notice in the Description page of Project Settings.

#include "ARGame/AI/ARBTService_CheckHealth.h"

#include "ARAICharacter.h"
#include "ARAIController.h"
#include "ARBase/NotNullPtr.h"
#include "ARGame/ARAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void UARBTService_CheckHealth::TickNode(UBehaviorTreeComponent& owner, uint8* node_memory,
										float delta)
{
	Super::TickNode(owner, node_memory, delta);

	// Check distance between AI awn and target actor.
	UBlackboardComponent* blackboard = owner.GetBlackboardComponent();
	if (!ensure(blackboard))
	{
		return;
	}

	NotNullPtr<AAIController> ai = owner.GetAIOwner();
	NotNullPtr character = Cast<AARAICharacter>(ai->GetPawn());

	auto* attributes = Cast<UARAttributeComponent>(
		character->GetComponentByClass(UARAttributeComponent::StaticClass()));
	if (!attributes)
	{
		return;
	}

	// Set the blackboard value on whether we are low health.
	bool low_health = attributes->LifeRatio() <= LowHealthThreshold;
	blackboard->SetValueAsBool(LowHealthKey.SelectedKeyName, low_health);
}