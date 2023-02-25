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
void AARTeleportProjectile::Tick(float delta)
{
	Super::Tick(delta);
}

void AARTeleportProjectile::OnBeginHit_Implementation(UPrimitiveComponent* hit_component,
													  AActor* other_actor,
													  UPrimitiveComponent* other_comp,
													  FVector normal_impulse, const FHitResult& hit)
{
	Super::OnBeginHit_Implementation(hit_component, other_actor, other_comp, normal_impulse, hit);

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
	instigator->TeleportTo(GetActorLocation(), instigator->GetActorRotation());
}
