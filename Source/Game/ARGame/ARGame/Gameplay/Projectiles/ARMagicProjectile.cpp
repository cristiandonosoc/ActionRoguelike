#include <ARGame/Gameplay/Projectiles/ARMagicProjectile.h>

#include <ARBase/ClientServerSplit.h>
#include <ARBase/DebugTools.h>

#include <Components/AudioComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include <Particles/ParticleSystem.h>
#include <Sound/SoundCue.h>

// Sets default values
AARMagicProjectile::AARMagicProjectile()
{
	INIT_BASE_CLIENT_SERVER_SPLIT();

	ExplosionParticle = CreateDefaultSubobject<UParticleSystem>("ExplosionParticle");

	TravelSound = CreateDefaultSubobject<USoundCue>("TravelSound");
	ExplosionSound = CreateDefaultSubobject<USoundCue>("ExplosionSound");
}

void AARMagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (TravelSound)
	{
		Audio->SetSound(TravelSound);
		Audio->Play(0.0f);
	}
}

void AARMagicProjectile::OnBeginHit_Implementation(UPrimitiveComponent* hit_component,
												   AActor* other_actor,
												   UPrimitiveComponent* other_comp,
												   FVector normal_impulse, const FHitResult& hit)
{
	Super::OnBeginHit_Implementation(hit_component, other_actor, other_comp, normal_impulse, hit);

	CLIENT_SERVER_CALL(OnBeginHit, hit, other_actor);
}