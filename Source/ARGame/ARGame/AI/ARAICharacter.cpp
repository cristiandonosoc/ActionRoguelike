// Fill out your copyright notice in the Description page of Project Settings.


#include "ARAICharacter.h"


#include "ARBase/NotNullPtr.h"


// Sets default values
AARAICharacter::AARAICharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance
	// if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AARAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AARAICharacter::Tick(float delta)
{
	Super::Tick(delta);
}

// Called to bind functionality to input
void AARAICharacter::SetupPlayerInputComponent(UInputComponent* player_input)
{
	Super::SetupPlayerInputComponent(player_input);
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
	AActor* proj = GetWorld()->SpawnActor<AActor>(PrimaryAttackProjectile.Get(), spawn_transform, params);

	// If the projectile could not be created, we mark the attack as failed.
	if (!proj)
	{
		return false;
	}
	
	return true;
}