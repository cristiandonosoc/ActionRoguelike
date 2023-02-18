// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "ARCharacter.generated.h"

class UAnimMontage;
class UARInteractionComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ACTIONROGUELIKE_API AARCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float delta) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* player_input) override;

	void MoveForward(float val);
	void MoveRight(float val);

	void PrimaryAttack();
	void PrimaryAttack_TimerElapsed();
	
	void PrimaryInteract();

protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnimation;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackDelay = 0.4f;

	FTimerHandle PrimaryAttack_TimerHandler;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UARInteractionComponent> InteractionComponent;

private:
	// This is where the player is looking from the camera at any given frame.
	// This is used by other systems to correctly point their target
	FVector CameraTarget;
};