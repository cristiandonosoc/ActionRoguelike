﻿// Fill out your copyright notice in the Description page of Project Settings.

#include <ARGame/ARBaseProjectile.h>

#include <Components/AudioComponent.h>
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystemComponent.h>

// Sets default values
AARBaseProjectile::AARBaseProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if
	// you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set the collision sphere as the root component for our projectile.
	CollisionSphere = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
	CollisionSphere->SetCollisionProfileName(TEXT("Projectile"));
	RootComponent = CollisionSphere;

	Effect = CreateDefaultSubobject<UParticleSystemComponent>("Effect");
	Effect->SetupAttachment(CollisionSphere);

	MuzzleEffect = CreateDefaultSubobject<UParticleSystem>("MuzzleEffect");

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = ProjectileSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bInitialVelocityInLocalSpace = true;

	Audio = CreateDefaultSubobject<UAudioComponent>("Audio");
	Audio->SetupAttachment(CollisionSphere);
}

void AARBaseProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CollisionSphere->OnComponentHit.AddDynamic(this, &AARBaseProjectile::OnBeginHitInternal);
}

// Called when the game starts or when spawned
void AARBaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	// We ignore the instigator when throwing the projectile.
	CollisionSphere->IgnoreActorWhenMoving(GetInstigator(), true);


	// If there is a muzzle effect, we play it.
	// TODO(cdc): This is really part of a weapon and probably should not be part of the projectile.
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, CollisionSphere, "Muzzle",
											   GetActorLocation(), GetActorRotation(), FVector(1),
											   EAttachLocation::KeepWorldPosition);
	}
}

void AARBaseProjectile::OnBeginHitInternal(UPrimitiveComponent* hit_component, AActor* other_actor,
										   UPrimitiveComponent* other_comp, FVector normal_impulse,
										   const FHitResult& hit)
{
	if (other_actor == nullptr)
	{
		return;
	}

	// We check to see if we're not colliding with the instigator.
	if (APawn* instigator = GetInstigator())
	{
		if (instigator == other_actor)
		{
			return;
		}
	}

	// We simply forward the call over to the derived components, either via blueprints or via C++.
	OnBeginHit(hit_component, other_actor, other_comp, normal_impulse, hit);
}

// Called every frame
void AARBaseProjectile::Tick(float delta)
{
	Super::Tick(delta);
}