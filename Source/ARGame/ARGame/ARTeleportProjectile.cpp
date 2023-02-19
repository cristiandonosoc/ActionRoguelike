// Fill out your copyright notice in the Description page of Project Settings.


#include "ARTeleportProjectile.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Templates/NonNullPointer.h"

// Sets default values
AARTeleportProjectile::AARTeleportProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if
	// you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere->OnComponentHit.AddDynamic(this, &AARTeleportProjectile::OnActorHit);

	ExplosionEffect = CreateDefaultSubobject<UParticleSystemComponent>("ExplosionEffect");
	ExplosionEffect->SetupAttachment(CollisionSphere);
	ExplosionEffect->DeactivateSystem();
}

void AARTeleportProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called when the game starts or when spawned
void AARTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionSphere->IgnoreActorWhenMoving(GetInstigator(), true);
	ExplosionEffect->DeactivateSystem();

	GetWorldTimerManager().SetTimer(FlyingTimerHandle, this,
									&AARTeleportProjectile::ExplosionTimerStart, FlyingTimerDelay,
									false);
}


// Called every frame
void AARTeleportProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AARTeleportProjectile::OnActorHit(UPrimitiveComponent* hit_component, AActor* other_actor,
									   UPrimitiveComponent* other_component, FVector normal,
									   const FHitResult& hit)
{
	// Check if we're colliding with the instigator.
	// If so, we ignore the collision.
	if (APawn* instigator = GetInstigator())
	{
		AActor* actor = Cast<AActor>(instigator);
		if (actor == other_actor)
		{
			return;
		}
	}

	ExplosionTimerStart();
}

void AARTeleportProjectile::ExplosionTimerStart()
{
	// Deactivate the flying timer in any case.
	FTimerManager& timer_manager = GetWorldTimerManager();
	timer_manager.ClearTimer(FlyingTimerHandle);

	ExplosionEffect->ActivateSystem(true);
	timer_manager.SetTimer(ExplosionTimerHandle, this, &AARTeleportProjectile::ExplosionTimerEnd,
						   ExplosionTimerDelay, false);
}

void AARTeleportProjectile::ExplosionTimerEnd()
{
	TeleportInstigator();
	Destroy();
}

void AARTeleportProjectile::TeleportInstigator()
{
	TNonNullPtr<APawn> instigator = GetInstigator();
	FTransform new_transform = FTransform(GetActorLocation());
	instigator->SetActorTransform(new_transform);
}