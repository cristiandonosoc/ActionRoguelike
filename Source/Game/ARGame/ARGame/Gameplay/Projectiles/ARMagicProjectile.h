#pragma once

#include <ARBase/ClientServerSplit.h>
#include <ARGame/Gameplay/Projectiles/ARBaseProjectile.h>

#if AR_BUILD_CLIENT
#include <ARGameClient/Gameplay/Projectiles/MagicProjectileClient.h>
#endif // AR_BUILD_CLIENT
#if AR_BUILD_SERVER
#include <ARGameServer/Gameplay/Projectiles/MagicProjectileServer.h>
#endif // AR_BUILD_SERVER

#include <CoreMinimal.h>
#include <GameplayTagContainer.h>

#include "ARMagicProjectile.generated.h"

class UARActionEffect;
class UParticleSystem;
class USoundCue;

UCLASS()
class ARGAME_API AARMagicProjectile : public AARBaseProjectile
{
	GENERATED_BODY()
	GENERATED_BASE_CLIENT_SERVER_SPLIT(AARMagicProjectile, ar::client::MagicProjectileClient,
									   ar::server::MagicProjectileServer);

public:
	// Sets default values for this actor's properties
	AARMagicProjectile();

public:
	float GetDamage() const { return Damage; }
	const FGameplayTag& GetParryTag() const { return ParryTag; }

	const TSubclassOf<UARActionEffect> GetActionEffect() const { return ActionEffect; }

	const TObjectPtr<UParticleSystem>& GetExplosionParticle() const { return ExplosionParticle; }
	const TObjectPtr<USoundCue>& GetExplosionSound() const { return ExplosionSound; }

public:
	// INTERFACE_BEGIN(AActor)
	virtual void BeginPlay() override;
	// INTERFACE_BEGIN(AActor)

	virtual void OnBeginHit_Implementation(UPrimitiveComponent* hit_component, AActor* other_actor,
										   UPrimitiveComponent* other_comp, FVector normal_impulse,
										   const FHitResult& hit) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Attacks")
	float Damage = 80;

	UPROPERTY(EditDefaultsOnly, Category = "Attacks")
	FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly, Category = "Attacks")
	TSubclassOf<UARActionEffect> ActionEffect;

	UPROPERTY(EditAnywhere, Category = "Particles")
	TObjectPtr<UParticleSystem> ExplosionParticle;

	UPROPERTY(EditAnywhere, Category = "Audio")
	TObjectPtr<USoundCue> TravelSound;

	UPROPERTY(EditAnywhere, Category = "Audio")
	TObjectPtr<USoundCue> ExplosionSound;
};