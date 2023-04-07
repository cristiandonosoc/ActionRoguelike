// Fill out your copyright notice in the Description page of Project Settings.

#include <ARGame/Gameplay/ARCharacter.h>

#include <ARGame/ARDebugCategories.h>
#include <ARGame/Gameplay/Base/ARPlayerState.h>
#include <ARGame/Gameplay/Components/ARActionComponent.h>
#include <ARGame/Gameplay/Components/ARAttributeComponent.h>
#include <ARGame/Gameplay/Components/ARInteractionComponent.h>
#include <ARGame/Gameplay/Projectiles/ARBaseProjectile.h>

#include <Camera/CameraComponent.h>
#include <Components/BoxComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include <Particles/ParticleSystem.h>


// Sets default values
AARCharacter::AARCharacter()
{
	INIT_BASE_CLIENT_SERVER_SPLIT();

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
	InteractionBox = CreateDefaultSubobject<UBoxComponent>("InteractionBox");
	InteractionBox->SetupAttachment(RootComponent);

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

	FString prefix;
	switch (GetWorld()->GetNetMode())
	{
	case NM_Client:
		// GPlayInEditorID 0 is always the server, so 1 will be first client.
		// You want to keep this logic in sync with GeneratePIEViewportWindowTitle and
		// UpdatePlayInEditorWorldDebugString
		prefix = FString::Printf(TEXT("Client %d"), GPlayInEditorID);
		break;
	case NM_DedicatedServer:
	case NM_ListenServer:
		prefix = FString::Printf(TEXT("Server"));
		break;
	case NM_Standalone:
		break;
	default:
		break;
	}


	FString line("*******************************************************************************\n"
				 "*******************************************************************************\n"
				 "*******************************************************************************");

	UE_LOG(LogAR_PlayerCharacter, Log, TEXT("%s\n%s (%s) -> IsServer: %d, IsClient: %d\n%s"), *line,
		   *prefix, *GetNameSafe(this), ARClientServerGlobals::RunningInServer(this),
		   ARClientServerGlobals::RunningInClient(this), *line);
}

FVector AARCharacter::GetPawnViewLocation() const
{
	return Camera->GetComponentLocation();
}

// Called to bind functionality to input
void AARCharacter::SetupPlayerInputComponent(UInputComponent* player_input)
{
	Super::SetupPlayerInputComponent(player_input);

	CLIENT_ONLY_CALL(SetupPlayerInput, player_input);
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

	debug::DrawDirectionalArrow(ar::PLAYER_CHARACTER, world, start, actor_forward, draw_scale,
								FColor::Yellow, thickness);
	debug::DrawDirectionalArrow(ar::PLAYER_CHARACTER, world, start, controller_forward, draw_scale,
								FColor::Green, thickness);
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
			debug::DrawSphere(ar::PLAYER_CHARACTER, character.GetWorld(), out_hit.Location, 20.0f,
							  16, FColor::Orange, 2);
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
	if (HasAuthority())
	{
		debug::DrawText(ar::ALWAYS, GetWorld(),
						FString::Printf(TEXT("%s LOCATION: %s"), *GetNameSafe(this),
										*GetActorLocation().ToString()),
						FColor::Purple);

		debug::DrawBox(ar::ALWAYS, GetWorld(), GetActorLocation(), FVector(50, 50, 75),
					   FColor::Purple, 2);
	}

	if (debug::IsCategoryEnabled(ar::PLAYER_CHARACTER))
	{
		DisplayCharacterRotation(*this);
	}
}

void AARCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	CLIENT_ONLY_CALL(NotifyControllerChanged);
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
	CLIENT_ONLY_CALL(OnHealthChanged, payload);
}