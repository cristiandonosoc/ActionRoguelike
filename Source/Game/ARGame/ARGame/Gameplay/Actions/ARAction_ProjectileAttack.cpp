#include <ARGame/Gameplay/Actions/ARAction_ProjectileAttack.h>

#include <ARGame/Gameplay/ARCharacter.h>
#include <ARGame/Gameplay/Components/ARActionComponent.h>
#include <ARGame/Gameplay/Projectiles/ARBaseProjectile.h>

UARAction_ProjectileAttack::UARAction_ProjectileAttack()
{
	INIT_BASE_CLIENT_SERVER_SPLIT();
}
FPredictedStartActionContext
UARAction_ProjectileAttack::OnClientPredictStart_Implementation(AActor* instigator)
{
	CHECK_RUNNING_ON_CLIENT(GetOwningComponent());
	auto context = Super::OnClientPredictStart_Implementation(instigator);

	GetClientSplit()->PredictStart(instigator, &context);
	return context;
}

void UARAction_ProjectileAttack::OnServerStart_Implementation(
	AActor* instigator, const FPredictedStartActionContext& context)
{
	Super::OnServerStart_Implementation(instigator, context);

	NotNullPtr character = Cast<AARCharacter>(instigator);
	GetServerSplit()->Start(character, context.Location, context.Rotation);
}