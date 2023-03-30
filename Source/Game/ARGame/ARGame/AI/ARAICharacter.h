// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <ARGame/Gameplay/Projectiles/ARBaseProjectile.h>

#include <CoreMinimal.h>
#include <GameFramework/Character.h>

#include "ARAICharacter.generated.h"

class UARActionComponent;
class UAREnemyHealthBarWidget;
struct FOnHealthChangedPayload;
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

	UFUNCTION()
	void OnHealthChanged(const FOnHealthChangedPayload& payload);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TObjectPtr<UARAttributeComponent> Attributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions")
	TObjectPtr<UARActionComponent> Actions;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UPawnSensingComponent> PawnSenses;

	UPROPERTY(EditAnywhere, Category = "Attacks")
	TSubclassOf<AARBaseProjectile> PrimaryAttackProjectile;

	UPROPERTY(EditAnywhere, Category = "Attacks", meta = (ClampMin = 0.0, ClampMax = 10.0f))
	float PrimaryAttackSpread = 2.0f;

	TObjectPtr<UAREnemyHealthBarWidget> HealthBarWidget;
};