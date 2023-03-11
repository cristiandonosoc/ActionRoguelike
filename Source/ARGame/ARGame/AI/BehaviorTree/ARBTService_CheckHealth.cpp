#include <ARGame/AI/BehaviorTree/ARBTService_CheckHealth.h>

#include <ARBase/NotNullPtr.h>
#include <ARGame/AI/ARAICharacter.h>
#include <ARGame/AI/ARAIController.h>
#include <ARGame/Gameplay/ARAttributeComponent.h>

#include <BehaviorTree/BlackboardComponent.h>

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