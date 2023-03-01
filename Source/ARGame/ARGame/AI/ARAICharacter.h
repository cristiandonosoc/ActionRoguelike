// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ARGame/ARBaseProjectile.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "ARAICharacter.generated.h"

class AARBaseProjectile;
class UARAttributeComponent;
class UPawnSensingComponent;

UCLASS()
class ARGAME_API AARAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARAICharacter();

	virtual void BeginPlay() override;

	virtual bool PerformPrimaryAttack(const AActor& target);

protected:
	UFUNCTION()
	void OnSeePawn(APawn* pawn);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TObjectPtr<UARAttributeComponent> Attributes;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UPawnSensingComponent> PawnSenses;

	UPROPERTY(EditAnywhere, Category = "Attacks")
	TSubclassOf<AARBaseProjectile> PrimaryAttackProjectile;
};