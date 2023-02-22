// Fill out your copyright notice in the Description page of Project Settings.


#include "ARAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Templates/NonNullPointer.h"

void AARAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensure(BehaviorTree))
	{
		RunBehaviorTree(BehaviorTree);
	}

	TNonNullPtr<APawn> player_pawn = UGameplayStatics::GetPlayerPawn(this, 0);
	TNonNullPtr<UBlackboardComponent> blackboard = GetBlackboardComponent();
	blackboard->SetValueAsVector("MoveToLocation", player_pawn->GetActorLocation());
	blackboard->SetValueAsObject("TargetActor", player_pawn);
}
