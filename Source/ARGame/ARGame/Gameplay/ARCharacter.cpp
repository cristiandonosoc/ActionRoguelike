// Fill out your copyright notice in the Description page of Project Settings.

#include <ARGame/Gameplay/ARCharacter.h>

#include <ARGame/ARDebugCategories.h>
#include <ARGame/Gameplay/Base/ARPlayerState.h>
#include <ARGame/Gameplay/Components/ARActionComponent.h>
#include <ARGame/Gameplay/Components/ARAttributeComponent.h>
#include <ARGame/Gameplay/Components/ARInteractionComponent.h>
#include <ARGame/Gameplay/Projectiles/ARBaseProjectile.h>

#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include <Particles/ParticleSystem.h>
#include <Templates/NonNullPointer.h>

AR_REGISTER_DEBUG_CATEGORY(ARDebugCategories::PLAYER_CHARACTER, false,
						   "All the display about the player character");

// Sets default values
AARCharacter::AARCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance
	// if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create components.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	InteractionComponent = CreateDefaultSubobject<UARInteractionComponent>("InteractionComponent");

	Attributes = CreateDefaultSubobject<UARAttributeComponent>("Attributes");

	Actions = CreateDefaultSubobject<UARActionComponent>("Actions");
}

void AARCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Attributes->OnHealthChanged.AddDynamic(this, &AARCharacter::OnHealthChanged);
}

// Called when the game starts or when spawned
void AARCharacter::BeginPlay()
{
	Super::BeginPlay();
}

FVector AARCharacter::GetPawnViewLocation() const
{
	return Camera->GetComponentLocation();
}

// Called to bind functionality to input
void AARCharacter::SetupPlayerInputComponent(UInputComponent* player_input)
{
	Super::SetupPlayerInputComponent(player_input);

	player_input->BindAxis("MoveForward", this, &AARCharacter::MoveForward);
	player_input->BindAxis("MoveRight", this, &AARCharacter::MoveRight);

	player_input->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	player_input->BindAxis("Lookup", this, &APawn::AddControllerPitchInput);

	player_input->BindAction("PrimaryAttack", IE_Pressed, this, &AARCharacter::PrimaryAttack);
	player_input->BindAction("DashAttack", IE_Pressed, this, &AARCharacter::DashAttack);
	player_input->BindAction("UltimateAttack", IE_Pressed, this, &AARCharacter::UltimateAttack);

	player_input->BindAction("Sprint", IE_Pressed, this, &AARCharacter::SprintStart);
	player_input->BindAction("Sprint", IE_Released, this, &AARCharacter::SprintEnd);

	player_input->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	player_input->BindAction("Interact", IE_Pressed, this, &AARCharacter::PrimaryInteract);
}

namespace
{

void DisplayCharacterRotation(const AARCharacter& character)
{
	constexpr float draw_scale = 100.0f;
	constexpr float thickness = 5.0f;

	FVector start = character.GetActorLocation() + character.GetActorRightVector() * 100.0f;

	FVector actor_forward = start + character.GetActorForwardVector() * 100.0f;
	FVector controller_forward = start + character.GetControlRotation().Vector() * 100.0f;

	// Draw the lines.
	NotNullPtr<UWorld> world = character.GetWorld();

	ARDebugDraw::DirectionalArrow(ARDebugCategories::PLAYER_CHARACTER, world, start, actor_forward,
								  draw_scale, FColor::Yellow, 0, thickness);
	ARDebugDraw::DirectionalArrow(ARDebugCategories::PLAYER_CHARACTER, world, start,
								  controller_forward, draw_scale, FColor::Green, 0, thickness);
}

// ObtainCameraTarget obtains where the player is looking at. Useful for several calculations to
// be done for interactivity.
//
// It does this by sending a trace from the camera into the world and seeing what does it collide
// with. This permits a much more crisp targeting. If there is no collision, a position is projected
// to the camera so that the player still points into the "horizon".
FVector ObtainCameraTarget(const AARCharacter& character)
{
	// If there is no player controller, we simply return zero.
	APlayerController* pc = character.GetLocalViewingPlayerController();
	if (!pc)
	{
		return FVector::Zero();
	}

	FVector camera_location;
	FRotator camera_rotator;
	pc->GetPlayerViewPoint(camera_location, camera_rotator);

	const FVector& start = camera_location;
	FVector end = start + (character.GetControlRotation().Vector() * 5000.0f);

	FHitResult out_hit;

	FCollisionQueryParams params = {};
	params.AddIgnoredActor(&character);

	FCollisionObjectQueryParams obj_params = {};
	obj_params.AddObjectTypesToQuery(ECC_WorldStatic);
	obj_params.AddObjectTypesToQuery(ECC_WorldDynamic);
	obj_params.AddObjectTypesToQuery(ECC_Pawn);
	if (character.GetWorld()->LineTraceSingleByObjectType(out_hit, start, end, obj_params, params))
	{
		if (AActor* hit_actor = out_hit.GetActor(); hit_actor)
		{
			ARDebugDraw::Sphere(ARDebugCategories::PLAYER_CHARACTER, character.GetWorld(),
								out_hit.Location, 20.0f, 16, FColor::Orange, 0, 2);
			return out_hit.Location;
		}
	}

	// If we did not hit anything, it most likely means that we pointed to somewhere with no
	// objects, like the sky. In that case we want to return the end of the collision query request,
	// which is projecting the camera view forward, so that the player doesn't end up pointing to
	// an unexpected random place.
	return end;
}

} // namespace

// Called every frame
void AARCharacter::Tick(float delta)
{
	Super::Tick(delta);

	CameraTarget = ObtainCameraTarget(*this);

	if (ARDebugDraw::IsCategoryEnabled(ARDebugCategories::PLAYER_CHARACTER))
	{
		DisplayCharacterRotation(*this);
	}
}


void AARCharacter::MoveForward(float val)
{
	// We remove the pitch and roll so that we don't mess the movement of the camera.
	FRotator rotator = GetControlRotation();
	rotator.Pitch = 0.0f;
	rotator.Roll = 0.0f;

	AddMovementInput(rotator.Vector(), val);
}

void AARCharacter::MoveRight(float val)
{
	// We remove the pitch and roll so that we don't mess the movement of the camera.
	FRotator rotator = GetControlRotation();
	rotator.Pitch = 0.0f;
	rotator.Roll = 0.0f;

	// X - Forward.
	// Y - Right.
	// Z - Up.
	FVector Right = FRotationMatrix(rotator).GetScaledAxis(EAxis::Y);
	AddMovementInput(Right, val);
}

namespace
{

void TryFireProjectile(AARCharacter* character, const FString& name,
					   const TSubclassOf<AARBaseProjectile>& projectile_class)
{
	if (ensure(projectile_class))
	{
		character->ProjectileAnimationStart(projectile_class);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s not set"), *name);
	}
}

} // namespace

void AARCharacter::PrimaryAttack()
{
	TryFireProjectile(this, TEXT("PrimaryAttack"), PrimaryAttackProjectile);
}

void AARCharacter::DashAttack()
{
	TryFireProjectile(this, TEXT("DashAttack"), DashAttackProjectile);
}

void AARCharacter::UltimateAttack()
{
	TryFireProjectile(this, TEXT("UltimateAttack"), UltimateAttackProjectile);
}

void AARCharacter::SprintStart()
{
	Actions->StartAction("sprint", this);
}

void AARCharacter::SprintEnd()
{
	Actions->StopAction("sprint", this);
}

void AARCharacter::ProjectileAnimationStart(const TSubclassOf<AARBaseProjectile>& projectile_class)
{
	// We play the attack animation and then we setup a timer to spawn the projectile once we're
	// done with that animation.
	// TODO(cdc): Use anim notify.
	CurrentProjectileClass = projectile_class;
	PlayAnimMontage(AttackAnimation);
	GetWorldTimerManager().SetTimer(ProjectileTimerHandle, this,
									&AARCharacter::ProjectileAnimationEnd, AttackDelay, false);
}

void AARCharacter::ProjectileAnimationEnd()
{
	check(CurrentProjectileClass);

	auto projectile_class = CurrentProjectileClass;
	CurrentProjectileClass = nullptr;

	// Obtain the place in the hand where we will spawn from.
	FVector hand_location = GetMesh()->GetSocketLocation("Muzzle_01");

	// We make it spawn with the rotation of the camera target.
	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(hand_location, CameraTarget);

	FTransform spawn_transform = FTransform(rotation, hand_location);

	FActorSpawnParameters params = {};
	TNonNullPtr<APawn> pawn = Cast<APawn>(this);
	params.Instigator = pawn;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(projectile_class.Get(), spawn_transform, params);
}

void AARCharacter::PrimaryInteract()
{
	if (ensureAlways(InteractionComponent))
	{
		InteractionComponent->PrimaryInteract(CameraTarget);
	}
}

int32 AARCharacter::GetCurrentCredits_Implementation()
{
	NotNullPtr game_state = GetPlayerStateChecked<AARPlayerState>();
	return game_state->GetCredits();
}

int32 AARCharacter::AddCredits_Implementation(int32 credits)
{
	check(credits > 0);
	NotNullPtr game_state = GetPlayerStateChecked<AARPlayerState>();
	return game_state->ChangeCredits(credits);
}

bool AARCharacter::PayCredits_Implementation(int32 price)
{
	check(price > 0);
	NotNullPtr game_state = GetPlayerStateChecked<AARPlayerState>();
	if (game_state->GetCredits() < price)
	{
		return false;
	}
	game_state->ChangeCredits(-price);
	return true;
}

void AARCharacter::OnHealthChanged(const FOnHealthChangedPayload& payload)
{
	if (payload.Killed())
	{
		DisableInput(Cast<APlayerController>(GetController()));
	}


	// Attempt to set the flash effect, camera shake, etc.
	if (payload.ActualDelta < 0.0f)
	{
		if (USkeletalMeshComponent* mesh = GetMesh())
		{
			mesh->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
		}

		if (CameraShake)
		{
			GetLocalViewingPlayerController()->PlayerCameraManager->PlayWorldCameraShake(
				GetWorld(), CameraShake.Get(), GetActorLocation(), 0.0f, 10000.0f, 0.1f);
		}
	}
}