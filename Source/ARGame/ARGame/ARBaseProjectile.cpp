// Fill out your copyright notice in the Description page of Project Settings.


#include "ARBaseProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AARBaseProjectile::AARBaseProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if
	// you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if
	// you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set the collision sphere as the root component for our projectile.
	CollisionSphere = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
	RootComponent = CollisionSphere;

	Effect = CreateDefaultSubobject<UParticleSystemComponent>("Effect");
	Effect->SetupAttachment(CollisionSphere);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 1000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bInitialVelocityInLocalSpace = true;
}

// Called when the game starts or when spawned
void AARBaseProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AARBaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}