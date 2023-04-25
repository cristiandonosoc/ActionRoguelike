#pragma once

#include <ARBase/ClientServerSplit.h>
#include <ARGame/Gameplay/Projectiles/ARBaseProjectile.h>
#if AR_BUILD_CLIENT
#include <ARGameClient/AI/AICharacterClient.h>
#endif // AR_BUILD_CLIENT
#if AR_BUILD_SERVER
#include <ARGameServer/AI/AICharacterServer.h>
#endif // AR_BUILD_SERVER

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
	GENERATED_BASE_CLIENT_SPLIT(AARAICharacter, ar::client::AICharacterClient);
	GENERATED_BASE_SERVER_SPLIT(AARAICharacter, ar::server::AICharacterServer);

public:
	AARAICharacter();

public:
	// INTERFACE_BEGIN(ACharacter)
	virtual void BeginPlay() override;
	// INTERFACE_END(ACharacter)
	
	// virtual bool PerformPrimaryAttack(const AActor& target);

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
};