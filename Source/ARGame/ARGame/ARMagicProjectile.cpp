// Fill out your copyright notice in the Description page of Project Settings.

#include <ARGame/ARMagicProjectile.h>

#include <ARGame/ARAttributeComponent.h>
#include <ARBase/BuildDefines.h>

#include <Components/AudioComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystem.h>
#include <Sound/SoundCue.h>

// Sets default values
AARMagicProjectile::AARMagicProjectile()
{
	ExplosionParticle = CreateDefaultSubobject<UParticleSystem>("ExplosionParticle");

	TravelSound = CreateDefaultSubobject<USoundCue>("TravelSound");
	ExplosionSound = CreateDefaultSubobject<USoundCue>("ExplosionSound");
}

// Called when the game starts or when spawned
void AARMagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (TravelSound)
	{
		Audio->SetSound(TravelSound);
		Audio->Play(0.0f);
	}

}

// Called every frame
void AARMagicProjectile::Tick(float delta)
{
	Super::Tick(delta);
}

namespace
{

template <typename... Args>
void Debug_DrawDebugSphere(Args... args)
{
#if AR_BUILD_DEBUG
	DrawDebugSphere(args...);
#endif // AR_BUILD_DEBUG
}

} // namespace

void AARMagicProjectile::OnBeginHit_Implementation(UPrimitiveComponent* hit_component,
												   AActor* other_actor,
												   UPrimitiveComponent* other_comp,
												   FVector normal_impulse, const FHitResult& hit)
{
	Super::OnBeginHit_Implementation(hit_component, other_actor, other_comp, normal_impulse, hit);

	// We see if the object we hit can receive damage.
	if (other_actor)
	{
		if (auto* attributes = Cast<UARAttributeComponent>(
				other_actor->GetComponentByClass(UARAttributeComponent::StaticClass())))
		{
			// We forward our instigator forward.
			attributes->ApplyHealthChange(GetInstigator(), -Damage); // We apply a negative delta.
		}
	}

	FVector location = GetActorLocation();
	FRotator rotation = GetActorRotation();

	// We draw the debug collision.
	Debug_DrawDebugSphere(GetWorld(), location, 20, 16, FColor::Yellow, false, 1, 0, 1);

	// Spawn a particle effect for the collision.
	if (ensure(ExplosionParticle))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, location, rotation);
	}

	// Play the sound.
	if (ensure(ExplosionSound))
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, location, rotation);
	}

	// We destroy this object as it is not needed anymore.
	Destroy();
}
