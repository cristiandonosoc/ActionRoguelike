// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ARGame/ARBaseProjectile.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "ARAICharacter.generated.h"

class AARBaseProjectile;

UCLASS()
class ARGAME_API AARAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARAICharacter();

	// Called every frame
	virtual void Tick(float delta) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* player_input) override;

	virtual bool PerformPrimaryAttack(const AActor& target);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AARBaseProjectile> PrimaryAttackProjectile;
};
