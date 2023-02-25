// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ARBaseItem.generated.h"

UCLASS()
class ARGAME_API AARBaseItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AARBaseItem();
	
	// Called every frame
	virtual void Tick(float delta) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
