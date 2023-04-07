#include <ARGameClient/Gameplay/MovementManager.h>

#include <ARGame/Gameplay/ARCharacter.h>
#include <ARGame/Gameplay/Components/ARActionComponent.h>
#include <ARGame/Gameplay/Components/ARInteractionComponent.h>
#include <ARGameClient/Gameplay/CharacterClient.h>

namespace ar
{
namespace client
{

namespace
{

using AxisCallback = void (MovementManager::*)(float dt);

// BindAxis goes a bit into the details of how axis are bound because it only exposes UObject
// binding and we want to bind a raw pointer.
void BindAxis(NotNullPtr<UInputComponent> input, const FName& axis_name,
			  NotNullPtr<MovementManager> movement_helper, AxisCallback callback)
{
	FInputAxisBinding ab(axis_name);
	auto& delegate = ab.AxisDelegate.GetDelegateForManualSet();

	// We bind a lambda into the delegate that uses the base as a weak pointer protection.
	// This protects into the (theoretical) case that the callbacks get called after our character
	// has been deleted.
	delegate.BindLambda(
		[movement_helper, callback,
		 weakBase = TWeakObjectPtr<AARCharacter>(movement_helper->GetBase())](float dt)
		{
			// If the actor is still alive, then the movement helper pointer is also valid.
			if (weakBase.IsValid())
			{
				(movement_helper->*callback)(dt);
			}
		});

	input->AxisBindings.Emplace(std::move(ab));
}

using ActionCallback = void (MovementManager::*)();

// BindAxis goes a bit into the details of how actions are bound because it only exposes UObject
// binding and we want to bind a raw pointer.
void BindAction(NotNullPtr<UInputComponent> input, const FName& action_name,
				const EInputEvent key_event, NotNullPtr<MovementManager> movement_helper,
				ActionCallback callback)
{
	FInputActionBinding ab(action_name, key_event);
	auto& delegate = ab.ActionDelegate.GetDelegateForManualSet();

	// We bind a lambda into the delegate that uses the base as a weak pointer protection.
	// This protects into the (theoretical) case that the callbacks get called after our character
	// has been deleted.
	delegate.BindLambda(
		[movement_helper, callback,
		 weakBase = TWeakObjectPtr<AARCharacter>(movement_helper->GetBase())]()
		{
			// If the actor is still alive, then the movement helper pointer is also valid.
			if (weakBase.IsValid())
			{
				(movement_helper->*callback)();
			}
		});

	input->AddActionBinding(std::move(ab));
}

} // namespace

void MovementManager::SetupPlayerInput(NotNullPtr<CharacterClient> char_client,
									   NotNullPtr<UInputComponent> input)
{
	CharacterClientSplit = char_client.Get();

	BindAxis(input, "MoveForward", this, &MovementManager::MoveForward);
	BindAxis(input, "MoveRight", this, &MovementManager::MoveRight);

	BindAxis(input, "Turn", this, &MovementManager::AddControllerYawInput);
	BindAxis(input, "Lookup", this, &MovementManager::AddControllerPitchInput);

	BindAction(input, "PrimaryAttack", IE_Pressed, this, &MovementManager::PrimaryAttack);
	BindAction(input, "DashAttack", IE_Pressed, this, &MovementManager::DashAttack);
	BindAction(input, "UltimateAttack", IE_Pressed, this, &MovementManager::UltimateAttack);

	BindAction(input, "Sprint", IE_Pressed, this, &MovementManager::SprintStart);
	BindAction(input, "Sprint", IE_Released, this, &MovementManager::SprintEnd);

	BindAction(input, "Jump", IE_Pressed, this, &MovementManager::Jump);
	BindAction(input, "Interact", IE_Pressed, this, &MovementManager::PrimaryInteract);
}

AARCharacter* MovementManager::GetBase()
{
	return CharacterClientSplit->GetBase();
}

void MovementManager::MoveForward(float val)
{
	// We remove the pitch and roll so that we don't mess the movement of the camera.
	FRotator rotator = GetBase()->GetControlRotation();
	rotator.Pitch = 0.0f;
	rotator.Roll = 0.0f;

	GetBase()->AddMovementInput(rotator.Vector(), val);
}

void MovementManager::MoveRight(float val)
{
	// We remove the pitch and roll so that we don't mess the movement of the camera.
	FRotator rotator = GetBase()->GetControlRotation();
	rotator.Pitch = 0.0f;
	rotator.Roll = 0.0f;

	// X - Forward.
	// Y - Right.
	// Z - Up.
	FVector Right = FRotationMatrix(rotator).GetScaledAxis(EAxis::Y);
	GetBase()->AddMovementInput(Right, val);
}

void MovementManager::AddControllerYawInput(float dt)
{
	GetBase()->AddControllerYawInput(dt);
}

void MovementManager::AddControllerPitchInput(float dt)
{
	GetBase()->AddControllerPitchInput(dt);
}

namespace
{

void TryRunProjectile(NotNullPtr<AARCharacter> character, const FName& action_name)
{
	character->GetActions()->ClientPredictStartAction(action_name, character);
}

} // namespace

void MovementManager::PrimaryAttack()
{
	TryRunProjectile(GetBase(), "PrimaryAttack");
}

void MovementManager::DashAttack()
{
	TryRunProjectile(GetBase(), "DashAttack");
}

void MovementManager::UltimateAttack()
{
	TryRunProjectile(GetBase(), "UltimateAttack");
}

void MovementManager::SprintStart()
{
	// GetBase()->GetActions()->StartAction("sprint", GetBase());
}

void MovementManager::SprintEnd()
{
	// GetBase()->GetActions()->StopAction("sprint", GetBase());
}

void MovementManager::Jump()
{
	GetBase()->Jump();
}

void MovementManager::PrimaryInteract()
{
	GetBase()->GetInteractionComponent()->PrimaryInteract();
}

} // namespace client
} // namespace ar