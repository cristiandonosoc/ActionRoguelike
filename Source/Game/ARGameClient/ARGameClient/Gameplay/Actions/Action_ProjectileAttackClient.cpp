#include <ARGameClient/Gameplay/Actions/Action_ProjectileAttackClient.h>

#include <ARGame/ARDebugCategories.h>
#include <ARGame/Gameplay/ARCharacter.h>
#include <ARGame/Gameplay/Actions/ARAction_ProjectileAttack.h>
#include <ARGame/Gameplay/Projectiles/ARBaseProjectile.h>

#include <Kismet/KismetMathLibrary.h>

namespace ar
{
namespace client
{

void Action_ProjectileAttackClient::Start(NotNullPtr<AActor> instigator)
{
	if (!ensure(GetBase()->GetProjectileClass()))
	{
		UE_LOG(LogAR_Projectiles, Error, TEXT("Projectile class is not set"));
		return;
	}

	AARCharacter* character = Cast<AARCharacter>(instigator.Get());
	if (!character)
	{
		UE_LOG(LogAR_Projectiles, Warning, TEXT("Instigator is not an AARCharacter"));
		return;
	}

	// We play the attack animation and then we setup a timer to spawn the projectile once we're
	// done with that animation.
	// TODO(cdc): Use anim notify.
	character->PlayAnimMontage(GetBase()->GetAttackAnimation());

	FTimerDelegate delegate;
	delegate.BindLambda(
		[this, character, weakBase = GetWeakBase()]()
		{
			if (weakBase.Get())
			{
				AttackTimerEnd(character);
			}
		});
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, std::move(delegate),
										   GetBase()->GetAttackAnimDelay(), false);
}

void Action_ProjectileAttackClient::AttackTimerEnd(NotNullPtr<AARCharacter> instigator)
{
	// Obtain the place in the hand where we will spawn from.
	FVector hand_location =
		instigator->GetMesh()->GetSocketLocation(GetBase()->GetHandSocketName());

	// We make it spawn with the rotation of the camera target.
	FRotator rotation =
		UKismetMathLibrary::FindLookAtRotation(hand_location, instigator->GetCameraTarget());

	GetBase()->ClientStop(instigator);
}

} // namespace client
} // namespace ar