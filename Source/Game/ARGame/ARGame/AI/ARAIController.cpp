// Fill out your copyright notice in the Description page of Project Settings.

#include <ARGame/AI/ARAIController.h>

#include <ARBase/BuildDefines.h>

#include <BehaviorTree/BlackboardComponent.h>
#include <Kismet/GameplayStatics.h>

void AARAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensure(BehaviorTree))
	{
		RunBehaviorTree(BehaviorTree);
	}
}

namespace
{

void Debug_DrawSetActorLocation(NotNullPtr<UWorld> world, NotNullPtr<AActor> actor)
{
#if AR_BUILD_DEBUG
	FString text = FString::Printf(TEXT("Player spotted: %s"), *actor->GetName());
	DrawDebugString(world, actor->GetActorLocation(), *text, nullptr, FColor::White, 4.0f);
#endif // AR_BUILD_DEBUG
}

} // namespace

void AARAIController::SetTargetActor(NotNullPtr<AActor> actor)
{
	NotNullPtr<UBlackboardComponent> blackboard = GetBlackboardComponent();
	blackboard->SetValueAsObject(TargetActorKey, actor);

	Debug_DrawSetActorLocation(GetWorld(), actor);
}