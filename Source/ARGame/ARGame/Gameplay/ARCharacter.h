// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <ARGame/Gameplay/ARGameplayInterface.h>

#include <ARBase/NotNullPtr.h>
#include <CoreMinimal.h>
#include <GameFramework/Character.h>

#include "ARCharacter.generated.h"

struct FOnHealthChangedPayload;
class AARBaseProjectile;
class UARActionComponent;
class UARAttributeComponent;
class UARInteractionComponent;

class UAnimMontage;
class UBoxComponent;
class UCameraComponent;
class UParticleSystem;
class USpringArmComponent;

UCLASS()
class ARGAME_API AARCharacter : public ACharacter, public IARCreditHolder
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARCharacter();

public:
	// Called every frame
	virtual void Tick(float delta) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* player_input) override;

	void MoveForward(float val);
	void MoveRight(float val);

	void PrimaryAttack();
	void DashAttack();
	void UltimateAttack();

	void SprintStart();
	void SprintEnd();

	void PrimaryInteract();

	// INTERFACE_BEGIN(IARCreditHolder)
	int32 GetCurrentCredits_Implementation() override;
	virtual int32 AddCredits_Implementation(int32 credits) override;
	virtual bool PayCredits_Implementation(int32 price) override;
	// INTERFACE_END(IARCreditHolder)

	NotNullPtr<UARActionComponent> GetActions() { return Actions.Get(); }
	NotNullPtr<UBoxComponent> GetInteractionBox() { return InteractionBox.Get(); }
	const FVector& GetCameraTarget() const { return CameraTarget; }

protected:
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual FVector GetPawnViewLocation() const override;

	UFUNCTION()
	void OnHealthChanged(const FOnHealthChangedPayload& payload);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	TObjectPtr<UARInteractionComponent> InteractionComponent;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	TObjectPtr<UBoxComponent> InteractionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	TObjectPtr<UARAttributeComponent> Attributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions")
	TObjectPtr<UARActionComponent> Actions;

	UPROPERTY(EditAnywhere, Category = "Attacks")
	TSubclassOf<AARBaseProjectile> PrimaryAttackProjectile;

	UPROPERTY(EditAnywhere, Category = "Attacks")
	TSubclassOf<AARBaseProjectile> DashAttackProjectile;

	UPROPERTY(EditAnywhere, Category = "Attacks")
	TSubclassOf<AARBaseProjectile> UltimateAttackProjectile;

	UPROPERTY(EditAnywhere, Category = "Animations")
	TSubclassOf<UCameraShakeBase> CameraShake;

private:
	// This is where the player is looking from the camera at any given frame.
	// This is used by other systems to correctly point their target
	FVector CameraTarget;
};