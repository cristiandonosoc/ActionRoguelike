// Fill out your copyright notice in the Description page of Project Settings.


#include "ARExplosiveBarrel.h"

#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AARExplosiveBarrel::AARExplosiveBarrel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if
	// you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	RootComponent = Mesh;

	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>("ExplosionForce");
	ExplosionForce->SetupAttachment(Mesh);
	ExplosionForce->SetAutoActivate(false);

	ExplosionForce->Radius = 750.0f;
	ExplosionForce->ImpulseStrength = 2500.0f;

	// Ignore mass of objects when doing explosion.
	ExplosionForce->bImpulseVelChange = true;

	ExplosionForce->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

// Called when the game starts or when spawned
void AARExplosiveBarrel::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void AARExplosiveBarrel::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void AARExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Mesh->OnComponentHit.AddDynamic(this, &AARExplosiveBarrel::OnActorHit);
}
void AARExplosiveBarrel::OnActorHit(UPrimitiveComponent *HitComponent, AActor *OtherActor,
									UPrimitiveComponent *OtherComponent, FVector Normal,
									const FHitResult &Hit)
{
	UE_LOG(LogTemp, Log, TEXT("OnActorHit in Explosive Barrel"));


	ExplosionForce->FireImpulse();

	UE_LOG(LogTemp, Warning, TEXT("Other Actor: %s, at time: %f"), *GetNameSafe(OtherActor),
		   GetWorld()->TimeSeconds);

	FString Temp = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, Temp, nullptr, FColor::Green, 2.0f, true);
}