#include <ARGame/Gameplay/Actions/ARAction_ProjectileAttack.h>

#include <ARGame/Gameplay/ARCharacter.h>
#include <ARGame/Gameplay/Projectiles/ARBaseProjectile.h>

UARAction_ProjectileAttack::UARAction_ProjectileAttack()
{
	INIT_BASE_CLIENT_SERVER_SPLIT();
}

void UARAction_ProjectileAttack::Start_Implementation(AActor* instigator)
{
	// Action has a special way of determining client/server, since it uses the instigator.
	if (ARClientServerGlobals::RunningInClient(instigator))
	{
		Super::Start_Implementation(instigator);
		GetClientSplit()->Start(instigator);
	}
}

void UARAction_ProjectileAttack::Server_Start_Implementation(AARCharacter* instigator,
															 const FVector& location,
															 const FRotator& rotation)
{
	check(instigator);

	if (ARClientServerGlobals::RunningInServer(instigator))
	{
		Super::Start_Implementation(instigator);
		GetServerSplit()->Start(instigator, location, rotation);
	}
}
