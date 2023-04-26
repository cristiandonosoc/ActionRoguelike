#include <ARGameClient/Gameplay/Projectiles/MagicProjectileClient.h>

#include <ARGame/ARDebugCategories.h>
#include <ARGame/Gameplay/ARAttributeFunctionLibrary.h>
#include <ARGame/Gameplay/Components/ARActionComponent.h>
#include <ARGame/Gameplay/Projectiles/ARMagicProjectile.h>
#include <Components/SphereComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include <Sound/SoundCue.h>

namespace ar
{
namespace client
{

namespace
{

void PlayHitEffects(NotNullPtr<AARMagicProjectile> base, const FVector& location,
					const FRotator& rotation)
{
	// Spawn a particle effect for the collision.
	if (ensure(base->GetExplosionParticle()))
	{
		UGameplayStatics::SpawnEmitterAtLocation(base->GetWorld(), base->GetExplosionParticle(),
												 location, rotation);
	}

	// Play the sound.
	if (ensure(base->GetExplosionSound()))
	{
		UGameplayStatics::PlaySoundAtLocation(base->GetWorld(), base->GetExplosionSound(), location,
											  rotation);
	}
}

} // namespace

void MagicProjectileClient::OnBeginHit(const FHitResult& hit, AActor* hit_actor)
{
	FVector location = GetBase()->GetActorLocation();
	FRotator rotation = GetBase()->GetActorRotation();

	// We draw the debug collision.
	debug::DrawSphere(ar::PROJECTILES, GetWorld(), location, 20, 16, FColor::Yellow, 1, 1);

	// Disable the collisions for this projectile now that we hit.
	GetBase()->CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PlayHitEffects(GetBase(), location, rotation);
}

} // namespace client
} // namespace ar