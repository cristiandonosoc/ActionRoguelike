// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "ARBase/NotNullPtr.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "CoreMinimal.h"

#include "ARAIController.generated.h"

class UBehaviorTree;

UCLASS()
class ARGAME_API AARAIController : public AAIController
{
	GENERATED_BODY()

public:
	void SetTargetActor(NotNullPtr<AActor> actor);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetActorKey = TEXT("TargetActor");

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
};