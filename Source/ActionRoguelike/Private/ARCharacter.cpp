// Fill out your copyright notice in the Description page of Project Settings.


// #include "ActionRoguelike/Public/ARCharacter.h"
#include "ARCharacter.h"

#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

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
}

// Called when the game starts or when spawned
void AARCharacter::BeginPlay() { Super::BeginPlay(); }

namespace ARCharacter_Private
{
	void VisualizeRotation(const AARCharacter &Character)
	{
		constexpr float DrawScale = 100.0f;
		constexpr float Thickness = 5.0f;

		FVector Start = Character.GetActorLocation() + (Character.GetActorRightVector() * 100.0f);

		FVector ActorForward = Start + (Character.GetActorForwardVector() * 100.0f);
		FVector ControllerForward = Start + (Character.GetControlRotation().Vector() * 100.0f);

		// Draw the lines.
		UWorld *World = Character.GetWorld();

		DrawDebugDirectionalArrow(World, Start, ActorForward, DrawScale, FColor::Yellow, false,
								  0.0f, 0, Thickness);
		DrawDebugDirectionalArrow(World, Start, ControllerForward, DrawScale, FColor::Green, false,
								  0.0f, 0, Thickness);
	}
} // namespace ARCharacter_Private

// Called every frame
void AARCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#ifdef UE_BUILD_DEBUG
	ARCharacter_Private::VisualizeRotation(*this);
#endif // UE_BUILD_DEBUG
}

// Called to bind functionality to input
void AARCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInput)
{
	Super::SetupPlayerInputComponent(PlayerInput);

	PlayerInput->BindAxis("MoveForward", this, &AARCharacter::MoveForward);
	PlayerInput->BindAxis("MoveRight", this, &AARCharacter::MoveRight);

	PlayerInput->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInput->BindAxis("Lookup", this, &APawn::AddControllerPitchInput);

	PlayerInput->BindAction("PrimaryAttack", IE_Pressed, this, &AARCharacter::PrimaryAttack);
	PlayerInput->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
}

void AARCharacter::MoveForward(float Value)
{
	// We remove the pitch and roll so that we don't mess the movement of the camera.
	FRotator Rotator = GetControlRotation();
	Rotator.Pitch = 0.0f;
	Rotator.Roll = 0.0f;

	AddMovementInput(Rotator.Vector(), Value);
}

void AARCharacter::MoveRight(float Value)
{
	// We remove the pitch and roll so that we don't mess the movement of the camera.
	FRotator Rotator = GetControlRotation();
	Rotator.Pitch = 0.0f;
	Rotator.Roll = 0.0f;

	// X - Forward.
	// Y - Right.
	// Z - Up.
	FVector Right = FRotationMatrix(Rotator).GetScaledAxis(EAxis::Y);
	AddMovementInput(Right, Value);
}

void AARCharacter::PrimaryAttack()
{
	if (!ProjectileClass)
	{
		return;
	}

	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FTransform SpawnTransform = FTransform(GetControlRotation(), HandLocation);
	FActorSpawnParameters SpawnParams = {};
	// TODO(cdc): Change.
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParams);
}