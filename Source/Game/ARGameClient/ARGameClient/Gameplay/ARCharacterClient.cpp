#include <ARGameClient/Gameplay/ARCharacterClient.h>

#include <ARGame/Gameplay/ARCharacter.h>
#include <ARGame/Gameplay/Components/ARActionComponent.h>
#include <ARGame/Gameplay/Components/ARInteractionComponent.h>


void ARCharacterClient::SetupPlayerInput(NotNullPtr<UInputComponent> input)
{
	MovementHelper.SetupPlayerInput(this, input);
}

void ARCharacterClient::NotifyControllerChanged()
{
	// We only care if we're locally controller, to tell the interaction component that it can
	// start tracking interactables (for player feedback).
	if (GetBase()->IsLocallyControlled())
	{
		GetBase()->GetInteractionComponent()->GetClientSplit()->NotifyIsLocalControlled();
	}
}

// MovementHelperManager ---------------------------------------------------------------------------

namespace ARCharacterClient_Private
{

namespace
{

using AxisCallback = void (MovementHelperManager::*)(float dt);

// BindAxis goes a bit into the details of how axis are bound because it only exposes UObject
// binding and we want to bind a raw pointer.
void BindAxis(NotNullPtr<UInputComponent> input, const FName& axis_name,
			  NotNullPtr<MovementHelperManager> movement_helper, AxisCallback callback)
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

using ActionCallback = void (MovementHelperManager::*)();

// BindAxis goes a bit into the details of how actions are bound because it only exposes UObject
// binding and we want to bind a raw pointer.
void BindAction(NotNullPtr<UInputComponent> input, const FName& action_name,
				const EInputEvent key_event, NotNullPtr<MovementHelperManager> movement_helper,
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

void MovementHelperManager::SetupPlayerInput(NotNullPtr<ARCharacterClient> char_client,
											 NotNullPtr<UInputComponent> input)
{
	ClientSplit = char_client.Get();

	BindAxis(input, "MoveForward", this, &MovementHelperManager::MoveForward);
	BindAxis(input, "MoveRight", this, &MovementHelperManager::MoveRight);

	BindAxis(input, "Turn", this, &MovementHelperManager::AddControllerYawInput);
	BindAxis(input, "Lookup", this, &MovementHelperManager::AddControllerPitchInput);

	BindAction(input, "PrimaryAttack", IE_Pressed, this, &MovementHelperManager::PrimaryAttack);
	BindAction(input, "DashAttack", IE_Pressed, this, &MovementHelperManager::DashAttack);
	BindAction(input, "UltimateAttack", IE_Pressed, this, &MovementHelperManager::UltimateAttack);

	BindAction(input, "Sprint", IE_Pressed, this, &MovementHelperManager::SprintStart);
	BindAction(input, "Sprint", IE_Released, this, &MovementHelperManager::SprintEnd);

	BindAction(input, "Jump", IE_Pressed, this, &MovementHelperManager::Jump);
	BindAction(input, "Interact", IE_Pressed, this, &MovementHelperManager::PrimaryInteract);
}

AARCharacter* MovementHelperManager::GetBase()
{
	return ClientSplit->GetBase();
}

void MovementHelperManager::MoveForward(float val)
{
	// We remove the pitch and roll so that we don't mess the movement of the camera.
	FRotator rotator = GetBase()->GetControlRotation();
	rotator.Pitch = 0.0f;
	rotator.Roll = 0.0f;

	GetBase()->AddMovementInput(rotator.Vector(), val);
}

void MovementHelperManager::MoveRight(float val)
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

void MovementHelperManager::AddControllerYawInput(float dt)
{
	GetBase()->AddControllerYawInput(dt);
}

void MovementHelperManager::AddControllerPitchInput(float dt)
{
	GetBase()->AddControllerPitchInput(dt);
}

namespace
{

void TryRunProjectile(NotNullPtr<AARCharacter> character, const FName& action_name)
{
	if (!character->GetActions()->StartAction(action_name, character, false))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s not set"), *action_name.ToString());
	}
}

} // namespace

void MovementHelperManager::PrimaryAttack()
{
	TryRunProjectile(GetBase(), "PrimaryAttack");
}

void MovementHelperManager::DashAttack()
{
	TryRunProjectile(GetBase(), "DashAttack");
}

void MovementHelperManager::UltimateAttack()
{
	TryRunProjectile(GetBase(), "UltimateAttack");
}

void MovementHelperManager::SprintStart()
{
	GetBase()->GetActions()->StartAction("sprint", GetBase());
}

void MovementHelperManager::SprintEnd()
{
	GetBase()->GetActions()->StopAction("sprint", GetBase());
}

void MovementHelperManager::Jump()
{
	GetBase()->Jump();
}

void MovementHelperManager::PrimaryInteract()
{
	GetBase()->GetInteractionComponent()->PrimaryInteract();
}

} // namespace ARCharacterClient_Private