#pragma once

#include <ARBase/NotNullPtr.h>
#include <ARGame/Gameplay/Actions/ARAction.h>
#include <CoreMinimal.h>

#include "ARAction_ProjectileAttack.generated.h"

class AARBaseProjectile;
class AARCharacter;

class UAnimMontage;

UCLASS()
class ARGAME_API UARAction_ProjectileAttack : public UARAction
{
	GENERATED_BODY()
public:
	UARAction_ProjectileAttack();

	virtual void Start_Implementation(AActor* instigator) override;

protected:
	void AttackTimerEnd(NotNullPtr<AARCharacter> instigator);

protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AARBaseProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Effects")
	FName HandSocketName = "Muzzle_01";

	UPROPERTY(EditAnywhere, Category = "Attacks")
	float AttackAnimDelay = 0.4f;

	UPROPERTY(EditAnywhere, Category = "Animations")
	TObjectPtr<UAnimMontage> AttackAnimation;

private:
	// This is the timer associated with the wait needed to spawn the projectile.
	// TODO(cdc): Use animation notifications.
	FTimerHandle TimerHandle;
};