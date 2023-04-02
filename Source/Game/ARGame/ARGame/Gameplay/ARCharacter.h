#pragma once

#include <ARBase/ClientServerSplit.h>
#include <ARBase/NotNullPtr.h>
#include <ARGame/Gameplay/ARGameplayInterface.h>

#if AR_BUILD_CLIENT
#include <ARGameClient/Gameplay/CharacterClient.h>
#endif // AR_BUILD_CLIENT
#if AR_BUILD_SERVER
#include <ARGameServer/Gameplay/CharacterServer.h>
#endif // AR_BUILD_SERVER
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
	GENERATED_BASE_CLIENT_SERVER_SPLIT(AARCharacter, ar::client::CharacterClient,
									   ar::server::CharacterServer);

public:
	// Sets default values for this character's properties
	AARCharacter();

public:
	NotNullPtr<UARActionComponent> GetActions() const { return Actions.Get(); }
	NotNullPtr<UBoxComponent> GetInteractionBox() const { return InteractionBox.Get(); }
	const FVector& GetCameraTarget() const { return CameraTarget; }
	NotNullPtr<UARInteractionComponent> GetInteractionComponent() const
	{
		return InteractionComponent.Get();
	}
	const TSubclassOf<UCameraShakeBase>& GetCameraShake() const { return CameraShake; }

public:
	// INTERFACE_BEGIN(AActor)
	virtual void Tick(float delta) override;
	// INTERFACE_END(AActor)

	// INTERFACE_BEGIN(APawn)
	virtual FVector GetPawnViewLocation() const override;
	virtual void NotifyControllerChanged() override;
	// INTERFACE_END(APawn)

	// INTERFACE_BEGIN(ACharacter)
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(UInputComponent* player_input) override;
	// INTERFACE_END(ACharacter)

	// INTERFACE_BEGIN(IARCreditHolder)
	int32 GetCurrentCredits_Implementation() override;
	virtual int32 AddCredits_Implementation(int32 credits) override;
	virtual bool PayCredits_Implementation(int32 price) override;
	// INTERFACE_END(IARCreditHolder)

protected:
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