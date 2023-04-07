#include <ARGameServer/Gameplay/Projectiles/MagicProjectileServer.h>

#include <ARGame/ARDebugCategories.h>
#include <ARGame/Gameplay/ARAttributeFunctionLibrary.h>
#include <ARGame/Gameplay/Actions/ARActionEffect.h>
#include <ARGame/Gameplay/Components/ARActionComponent.h>
#include <ARGame/Gameplay/Projectiles/ARMagicProjectile.h>
#include <Kismet/KismetMathLibrary.h>

namespace ar
{
namespace server
{

bool WasProjectileParried(NotNullPtr<AARMagicProjectile> base,
						  NotNullPtr<UARActionComponent> actions, const FHitResult& hit,
						  NotNullPtr<AActor> other_actor)
{
	// We check for parrying.
	if (!actions->GetActiveGameplayTags().HasTagExact(base->GetParryTag()))
	{
		return false;
	}

	// TODO(cdc): Spawn a projectile backwards.

	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(hit.Location, hit.TraceStart);

	debug::DrawDirectionalArrow(ar::PROJECTILES, base->GetWorld(), hit.TraceEnd, hit.TraceStart, 10,
								FColor::Blue, 2);

	FTransform spawn_transform = FTransform(rotation, hit.Location);

	FActorSpawnParameters params = {};
	params.Instigator = Cast<APawn>(other_actor.Get());
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	base->GetWorld()->SpawnActorAbsolute<AActor>(base->GetClass(), spawn_transform, params);

	return true;
}

void MagicProjectileServer::OnBeginHit(const FHitResult& hit, AActor* other_actor)
{
	// As soon we hit something, we mark this for destruction.
	// We give it some time so that the client side has some time to do the shenanigans.
	MarkForDestruction();

	if (!other_actor)
	{
		return;
	}

	// We check if the actor we hit wants to parry the projectile.
	auto* actions = other_actor->FindComponentByClass<UARActionComponent>();
	if (actions && WasProjectileParried(GetBase(), actions, hit, other_actor))
	{
		return;
	}

	// If we weren't parried we try to damage.
	UARAttributeFunctionLibrary::Server_ApplyDirectionalDamage(
		GetBase()->GetInstigator(), other_actor, GetBase()->GetDamage(), hit);

	// See if we need to add an effect.
	if (actions && GetBase()->GetActionEffect())
	{
		actions->AddAction(GetBase()->GetActionEffect(), GetBase()->GetInstigator());
	}
}

void MagicProjectileServer::MarkForDestruction()
{
	FTimerDelegate delegate;
	delegate.BindLambda(
		[weakBase = TWeakObjectPtr<AARMagicProjectile>(GetBase())]()
		{
			if (auto* base = weakBase.Get())
			{
				base->Destroy();
			}
		});

	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, std::move(delegate), kDestroyDelay,
										   false);
}


} // namespace server
} // namespace ar