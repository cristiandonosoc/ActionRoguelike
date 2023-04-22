#include <ARGameServer/Gameplay/Actions/Action_ProjectileAttackServer.h>

#include <ARGame/Gameplay/ARCharacter.h>
#include <ARGame/Gameplay/Actions/ARAction_ProjectileAttack.h>
#include <ARGame/Gameplay/Projectiles/ARBaseProjectile.h>

#include <Kismet/KismetMathLibrary.h>

namespace ar
{
namespace server
{

void Action_ProjectileAttackServer::Start(NotNullPtr<AARCharacter> instigator,
										  const FVector& location, const FRotator& rotation)
{
	check(GetBase()->GetProjectileClass());


	FTransform spawn_transform = FTransform(rotation, location);

	FActorSpawnParameters params = {};
	params.Instigator = instigator.Get();
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(GetBase()->GetProjectileClass().Get(), spawn_transform, params);

	// This is a one-off action, so we stop it.
	GetBase()->DispatchServerStop(instigator);
}

} // namespace server
} // namespace ar
