#include <ARGameServer/AI/AICharacterServer.h>

#include <ARGame/AI/ARAICharacter.h>
#include <ARGame/AI/ARAIController.h>
#include <ARGame/Gameplay/Components/ARAttributeComponent.h>

#include <BrainComponent.h>
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

namespace ar
{
namespace server
{

bool AICharacterServer::PerformPrimaryAttack(const AActor& target)
{
	if (!ensure(GetBase()->PrimaryAttackProjectile))
	{
		return false;
	}

	// Get the location where the character is supposed to attack from.
	FVector muzzle_location = GetBase()->GetMesh()->GetSocketLocation("Muzzle_01");
	if (muzzle_location.IsZero())
	{
		return false;
	}

	// We get the direction in which we need to shoot the projectile.
	FVector direction = target.GetActorLocation() - muzzle_location;
	direction.Normalize();

	FRotator rotation = direction.Rotation();

	// Add the spread
	float& spread = GetBase()->PrimaryAttackSpread;
	if (spread > 0.0f)
	{
		rotation.Pitch += FMath::RandRange(-spread, spread);
		rotation.Yaw += FMath::RandRange(-spread, spread);
	}

	FTransform spawn_transform = FTransform(rotation, muzzle_location);

	FActorSpawnParameters params = {};
	NotNullPtr pawn = Cast<APawn>(GetBase());
	params.Instigator = pawn;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* proj = GetWorld()->SpawnActor<AActor>(GetBase()->PrimaryAttackProjectile.Get(),
												  spawn_transform, params);

	// If the projectile could not be created, we mark the attack as failed.
	if (!proj)
	{
		return false;
	}

	return true;
}

void AICharacterServer::OnHealthChanged(const FOnHealthChangedPayload& payload)
{
	if (payload.Killed())
	{
		// Stop BT.
		NotNullPtr ai = Cast<AARAIController>(GetBase()->GetController());
		ai->GetBrainComponent()->StopLogic(TEXT("Killed"));

		GetBase()->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetBase()->GetCharacterMovement()->DisableMovement();

		// Destroy the character after a while.
		GetBase()->SetLifeSpan(10.0f);

		return;
	}

	// If there is an instigator, make the AI react to it.
	if (APawn* instigator = Cast<APawn>(payload.Instigator.Get()))
	{
		if (instigator != GetBase())
		{
			NotNullPtr ai = Cast<AARAIController>(GetBase()->GetController());
			ai->SetTargetActor(instigator);
		}
	}
}

} // namespace server
} // namespace ar