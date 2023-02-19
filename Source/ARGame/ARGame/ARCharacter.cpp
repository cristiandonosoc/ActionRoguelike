// Fill out your copyright notice in the Description page of Project Settings.


#include "ARCharacter.h"

#include "ARBase/BuildDefines.h"
#include "ARGame/ARInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Templates/NonNullPointer.h"

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

	this->bUseControllerRotationYaw = false;

	InteractionComponent = CreateDefaultSubobject<UARInteractionComponent>("InteractionComponent");
}

// Called when the game starts or when spawned
void AARCharacter::BeginPlay()
{
	Super::BeginPlay();
}

namespace
{

void Debug_DisplayCharacterRotation(const AARCharacter& character)
{
#if AR_BUILD_DEBUG
	constexpr float draw_scale = 100.0f;
	constexpr float thickness = 5.0f;

	FVector start = character.GetActorLocation() + (character.GetActorRightVector() * 100.0f);

	FVector actor_forward = start + (character.GetActorForwardVector() * 100.0f);
	FVector controller_forward = start + (character.GetControlRotation().Vector() * 100.0f);

	// Draw the lines.
	TNonNullPtr<UWorld> world = character.GetWorld();

	DrawDebugDirectionalArrow(world, start, actor_forward, draw_scale, FColor::Yellow, false, 0.0f,
							  0, thickness);
	DrawDebugDirectionalArrow(world, start, controller_forward, draw_scale, FColor::Green, false,
							  0.0f, 0, thickness);
#endif
}

void Debug_DisplayTarget(const AARCharacter& character, const FVector& target)
{
#if AR_BUILD_DEBUG
	DrawDebugSphere(character.GetWorld(), target, 20.0f, 16, FColor::Orange, false, 0, 0, 2);
#endif
}

// ObtainCameraTarget obtains where the player is looking at. Useful for several calculations to
// be done for interactivity.
//
// It does this by sending a trace from the camera into the world and seeing what does it collide
// with. This permits a much more crisp targeting. If there is no collision, a position is projected
// to the camera so that the player still points into the "horizon".
FVector ObtainCameraTarget(const AARCharacter& character)
{
	// Targeting.
	TNonNullPtr<APlayerController> pc = character.GetLocalViewingPlayerController();

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
			Debug_DisplayTarget(character, out_hit.Location);
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

	Debug_DisplayCharacterRotation(*this);
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
	player_input->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	player_input->BindAction("Interact", IE_Pressed, this, &AARCharacter::PrimaryInteract);
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

void AARCharacter::PrimaryAttack()
{
	if (!ProjectileClass)
	{
		return;
	}

	PlayAnimMontage(AttackAnimation);

	GetWorldTimerManager().SetTimer(PrimaryAttack_TimerHandler, this,
									&AARCharacter::PrimaryAttack_TimerElapsed, AttackDelay, false);
}

void AARCharacter::PrimaryAttack_TimerElapsed()
{
	// Obtain the place in the hand where we will spawn from.
	FVector hand_location = GetMesh()->GetSocketLocation("Muzzle_01");

	// We make it spawn with the rotation of the camera target.
	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(hand_location, CameraTarget);
	FTransform spawn_transform = FTransform(rotation, hand_location);

	FActorSpawnParameters params = {};
	TNonNullPtr<APawn> pawn = Cast<APawn>(this);
	params.Instigator = pawn;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(ProjectileClass, spawn_transform, params);
}

void AARCharacter::PrimaryInteract()
{
	if (InteractionComponent)
	{
		InteractionComponent->PrimaryInteract(CameraTarget);
	}
}