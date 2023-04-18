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

void Action_ProjectileAttackClient::PredictStart(
	NotNullPtr<AActor> instigator, NotNullPtr<FPredictedStartActionContext> out_context)
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

	// We only play the animation if we're not local.
	// If we're locally controlled, we play the animation as part of the prediction.
	if (!character->IsLocallyControlled())
	{
		// We play the attack animation and then we setup a timer to spawn the projectile once we're
		// done with that animation.
		// TODO(cdc): Use anim notify.
		character->PlayAnimMontage(GetBase()->GetAttackAnimation());
	}

	// Obtain the place in the hand where we will spawn from.
	out_context->Location = character->GetMesh()->GetSocketLocation(GetBase()->GetHandSocketName());

	// We make it spawn with the rotation of the camera target.
	out_context->Rotation =
		UKismetMathLibrary::FindLookAtRotation(out_context->Location, character->GetCameraTarget());
}

} // namespace client
} // namespace ar