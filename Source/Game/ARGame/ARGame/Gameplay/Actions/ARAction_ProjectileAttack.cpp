#include <ARGame/Gameplay/Actions/ARAction_ProjectileAttack.h>

#include <ARGame/Gameplay/ARCharacter.h>
#include <ARGame/Gameplay/Projectiles/ARBaseProjectile.h>
#include <Kismet/KismetMathLibrary.h>

UARAction_ProjectileAttack::UARAction_ProjectileAttack() {}

void UARAction_ProjectileAttack::Start_Implementation(AActor* instigator)
{
	Super::Start_Implementation(instigator);

	if (!ensure(ProjectileClass))
	{
		UE_LOG(LogTemp, Error, TEXT("Projectile class is not set"));
		return;
	}

	AARCharacter* character = Cast<AARCharacter>(instigator);
	if (!character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Instigator is not an AARCharacter"));
		return;
	}

	// We play the attack animation and then we setup a timer to spawn the projectile once we're
	// done with that animation.
	// TODO(cdc): Use anim notify.
	character->PlayAnimMontage(AttackAnimation);

	FTimerDelegate delegate;
	delegate.BindUObject(this, &UARAction_ProjectileAttack::AttackTimerEnd,
						 NotNullPtr<AARCharacter>(character));
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, delegate, AttackAnimDelay, false);
}

void UARAction_ProjectileAttack::AttackTimerEnd(NotNullPtr<AARCharacter> instigator)
{
	check(ProjectileClass);

	// Obtain the place in the hand where we will spawn from.
	FVector hand_location = instigator->GetMesh()->GetSocketLocation(HandSocketName);

	// We make it spawn with the rotation of the camera target.
	FRotator rotation =
		UKismetMathLibrary::FindLookAtRotation(hand_location, instigator->GetCameraTarget());

	FTransform spawn_transform = FTransform(rotation, hand_location);

	FActorSpawnParameters params = {};
	params.Instigator = instigator.Get();
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(ProjectileClass.Get(), spawn_transform, params);

	// This is a one-off action, so we stop it.
	Stop(instigator);
}