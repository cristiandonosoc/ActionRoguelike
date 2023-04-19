#pragma once

#include <ARBase/ClientServerSplit.h>
#include <ARGame/Gameplay/Actions/ARAction.h>
#if AR_BUILD_CLIENT
#include <ARGameClient/Gameplay/Actions/Action_ProjectileAttackClient.h>
#endif // AR_BUILD_CLIENT
#if AR_BUILD_SERVER
#include <ARGameServer/Gameplay/Actions/Action_ProjectileAttackServer.h>
#endif // AR_BUILD_SERVER

#include <CoreMinimal.h>

#include "ARAction_ProjectileAttack.generated.h"

class AARBaseProjectile;
class AARCharacter;
struct FPredictedStartActionContext;

class UAnimMontage;

UCLASS()
class ARGAME_API UARAction_ProjectileAttack : public UARAction
{
	GENERATED_BODY()
	GENERATED_BASE_CLIENT_SPLIT(UARAction_ProjectileAttack,
								ar::client::Action_ProjectileAttackClient);
	GENERATED_BASE_SERVER_SPLIT(UARAction_ProjectileAttack,
								ar::server::Action_ProjectileAttackServer);

public:
	UARAction_ProjectileAttack();

public:
	const TSubclassOf<AARBaseProjectile>& GetProjectileClass() const { return ProjectileClass; }
	const TObjectPtr<UAnimMontage>& GetAttackAnimation() const { return AttackAnimation; }
	const FName& GetHandSocketName() const { return HandSocketName; }

	float GetAttackAnimDelay() const { return AttackAnimDelay; }

public:
	// INTERFACE_BEGIN(UARAction)
	virtual FPredictedStartActionContext ClientPredictStart_Implementation(AActor* instigator);
	// virtual void ClientPredictStop_Implementation(AActor* instigator) override;
	// virtual void ClientStart_Implementation(AActor* instigator) override;
	// virtual void ClientStop_Implementation(AActor* instigator) override;
	virtual void ServerStart_Implementation(AActor* instigator,
											const FPredictedStartActionContext& context) override;
	// virtual void ServerStop_Implementation(AActor* instigator) override;
	//  INTERFACE_END(UARAction)

protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AARBaseProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Effects")
	FName HandSocketName = "Muzzle_01";

	// TODO(cdc): This *FOR SURE* doesn't have to be here.
	UPROPERTY(EditAnywhere, Category = "Attacks")
	float AttackAnimDelay = 0.4f;

	// TODO(cdc): This *FOR SURE* doesn't have to be here.
	UPROPERTY(EditAnywhere, Category = "Animations")
	TObjectPtr<UAnimMontage> AttackAnimation;
};