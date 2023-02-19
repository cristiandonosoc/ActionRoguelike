// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "ARCharacter.generated.h"

class AARBaseProjectile;
class UAnimMontage;
class UARInteractionComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ARGAME_API AARCharacter : public ACharacter
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
	void DashAttack();
	void UltimateAttack();

	void ProjectileAnimationStart(const TSubclassOf<AARBaseProjectile>& projectile_class);
	void ProjectileAnimationEnd();
	
	void PrimaryInteract();

protected:
	UPROPERTY(EditAnywhere, Category = "Attacks")
	TSubclassOf<AARBaseProjectile> PrimaryAttackProjectile;
	
	UPROPERTY(EditAnywhere, Category = "Attacks")
	TSubclassOf<AARBaseProjectile> DashAttackProjectile;

	UPROPERTY(EditAnywhere, Category = "Attacks")
	TSubclassOf<AARBaseProjectile> UltimateAttackProjectile;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnimation;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackDelay = 0.4f;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UARInteractionComponent> InteractionComponent;

private:
	// This is the timer associated with the wait needed to spawn the projectile.
	// TOOD(cdc): Use animation notifications.
	FTimerHandle ProjectileTimerHandle;

	// This is the class of the current projectile we're spawning.
	TSubclassOf<AARBaseProjectile> CurrentProjectileClass;
	
	// This is where the player is looking from the camera at any given frame.
	// This is used by other systems to correctly point their target
	FVector CameraTarget;
};