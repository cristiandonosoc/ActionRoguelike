#include <ARGame/Gameplay/Actions/ARAction_ProjectileAttack.h>

#include <ARGame/Gameplay/ARCharacter.h>
#include <ARGame/Gameplay/Components/ARActionComponent.h>
#include <ARGame/Gameplay/Projectiles/ARBaseProjectile.h>

UARAction_ProjectileAttack::UARAction_ProjectileAttack()
{
	INIT_BASE_CLIENT_SERVER_SPLIT();
}
FPredictedStartActionContext
UARAction_ProjectileAttack::ClientPredictStart_Implementation(AActor* instigator)
{
	CHECK_RUNNING_ON_CLIENT(GetOwningComponent());
	auto context = Super::ClientPredictStart_Implementation(instigator);

	GetClientSplit()->PredictStart(instigator, &context);
	return context;
}

// void UARAction_ProjectileAttack::ClientStart_Implementation(AActor* instigator)
// {
// 	CHECK_RUNNING_ON_CLIENT(GetOwningComponent());
// 	Super::ClientStart_Implementation(instigator);
// 	GetClientSplit()->Start(instigator);
// }

// void UARAction_ProjectileAttack::ServerStart_Implementation(AARCharacter* instigator,
//															 const FVector& location,
//															 const FRotator& rotation)
//{
//	check(instigator);
//
//	if (ARClientServerGlobals::RunningInServer(instigator))
//	{
//		Super::Start_Implementation(instigator);
//		GetServerSplit()->Start(instigator, location, rotation);
//	}
// }