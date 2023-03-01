// Fill out your copyright notice in the Description page of Project Settings.

#include "ARAICharacter.h"

#include "ARAIController.h"
#include "ARBase/NotNullPtr.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AARAICharacter::AARAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSenses = CreateDefaultSubobject<UPawnSensingComponent>("PawnSenses");
}

void AARAICharacter::BeginPlay()
{
	Super::BeginPlay();

	PawnSenses->OnSeePawn.AddDynamic(this, &AARAICharacter::OnSeePawn);
}

void AARAICharacter::OnSeePawn(APawn* pawn)
{
	NotNullPtr ai = Cast<AARAIController>(GetController());
	ai->SetTargetActor(pawn);
}

bool AARAICharacter::PerformPrimaryAttack(const AActor& target)
{
	if (!ensure(PrimaryAttackProjectile))
	{
		return false;
	}

	// Get the location where the character is supposed to attack from.
	FVector muzzle_location = GetMesh()->GetSocketLocation("Muzzle_01");
	if (muzzle_location.IsZero())
	{
		return false;
	}

	// We get the direction in which we need to shoot the projectile.
	FVector direction = target.GetActorLocation() - muzzle_location;
	direction.Normalize();

	FRotator rotation = direction.Rotation();

	FTransform spawn_transform = FTransform(rotation, muzzle_location);

	FActorSpawnParameters params = {};
	NotNullPtr pawn = Cast<APawn>(this);
	params.Instigator = pawn;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* proj =
		GetWorld()->SpawnActor<AActor>(PrimaryAttackProjectile.Get(), spawn_transform, params);

	// If the projectile could not be created, we mark the attack as failed.
	if (!proj)
	{
		return false;
	}

	return true;
}
