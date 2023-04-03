#include <ARGameClient/Gameplay/Projectiles/BaseProjectileClient.h>

#include <ARGame/Gameplay/Projectiles/ARBaseProjectile.h>
#include <Kismet/GameplayStatics.h>

namespace ar
{
namespace client
{

void BaseProjectileClient::BeginPlay()
{
	// If there is a muzzle effect, we play it.
	// TODO(cdc): This is really part of a weapon and probably should not be part of the projectile.
	if (GetBase()->GetMuzzleEffect())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GetBase()->GetMuzzleEffect(),
												 GetBase()->GetActorLocation(),
												 GetBase()->GetActorRotation());
	}
}


} // namespace client
} // namespace ar