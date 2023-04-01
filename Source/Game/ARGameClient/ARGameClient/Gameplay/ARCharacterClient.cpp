#include <ARGameClient/Gameplay/ARCharacterClient.h>

#include <ARGame/Gameplay/ARCharacter.h>
#include <ARGame/Gameplay/Components/ARActionComponent.h>
#include <ARGame/Gameplay/Components/ARInteractionComponent.h>

namespace
{

using AxisCallback = void (ARCharacterClient::*)(float dt);

// BindAxis goes a bit into the details of how axis are bound because it only exposes UObject
// binding and we want to bind a raw pointer.
void BindAxis(NotNullPtr<UInputComponent> input, const FName& axis_name,
			  NotNullPtr<ARCharacterClient> char_client, AxisCallback callback)
{
	FInputAxisBinding ab(axis_name);
	auto& delegate = ab.AxisDelegate.GetDelegateForManualSet();

	// We bind a lambda into the delegate that uses the base as a weak pointer protection.
	// This protects into the (theoretical) case that the callbacks get called after our character
	// has been deleted.
	delegate.BindLambda(
		[callback, weakBase = TWeakObjectPtr<AARCharacter>(char_client->GetBase())](float dt)
		{
			if (AARCharacter* base = weakBase.Get())
			{
				// Call the callback member function.
				(base->GetClientSplit().*callback)(dt);
			}
		});

	input->AxisBindings.Emplace(std::move(ab));
}

using ActionCallback = void (ARCharacterClient::*)();

// BindAxis goes a bit into the details of how actions are bound because it only exposes UObject
// binding and we want to bind a raw pointer.
void BindAction(NotNullPtr<UInputComponent> input, const FName& action_name,
				const EInputEvent key_event, NotNullPtr<ARCharacterClient> char_client,
				ActionCallback callback)
{
	FInputActionBinding ab(action_name, key_event);
	auto& delegate = ab.ActionDelegate.GetDelegateForManualSet();

	// We bind a lambda into the delegate that uses the base as a weak pointer protection.
	// This protects into the (theoretical) case that the callbacks get called after our character
	// has been deleted.
	delegate.BindLambda(
		[callback, weakBase = TWeakObjectPtr<AARCharacter>(char_client->GetBase())]()
		{
			if (AARCharacter* base = weakBase.Get())
			{
				// Call the callback member function.
				(base->GetClientSplit().*callback)();
			}
		});

	input->AddActionBinding(std::move(ab));
}

} // namespace

void ARCharacterClient::SetupPlayerInput(NotNullPtr<UInputComponent> input)
{
	BindAxis(input, "MoveForward", this, &ARCharacterClient::MoveForward);
	BindAxis(input, "MoveRight", this, &ARCharacterClient::MoveRight);

	BindAxis(input, "Turn", this, &ARCharacterClient::AddControllerYawInput);
	BindAxis(input, "Lookup", this, &ARCharacterClient::AddControllerPitchInput);

	BindAction(input, "PrimaryAttack", IE_Pressed, this, &ARCharacterClient::PrimaryAttack);
	BindAction(input, "DashAttack", IE_Pressed, this, &ARCharacterClient::DashAttack);
	BindAction(input, "UltimateAttack", IE_Pressed, this, &ARCharacterClient::UltimateAttack);

	BindAction(input, "Sprint", IE_Pressed, this, &ARCharacterClient::SprintStart);
	BindAction(input, "Sprint", IE_Released, this, &ARCharacterClient::SprintEnd);

	BindAction(input, "Jump", IE_Pressed, this, &ARCharacterClient::Jump);
	BindAction(input, "Interact", IE_Pressed, this, &ARCharacterClient::PrimaryInteract);
}

void ARCharacterClient::NotifyControllerChanged()
{
	// We only care if we're locally controller, to tell the interaction component that it can
	// start tracking interactables (for player feedback).
	if (GetBase()->IsLocallyControlled())
	{
		GetBase()->GetInteractionComponent()->GetClientSplit().NotifyIsLocalControlled();
	}
}

void ARCharacterClient::MoveForward(float val)
{
	// We remove the pitch and roll so that we don't mess the movement of the camera.
	FRotator rotator = GetBase()->GetControlRotation();
	rotator.Pitch = 0.0f;
	rotator.Roll = 0.0f;

	GetBase()->AddMovementInput(rotator.Vector(), val);
}

void ARCharacterClient::MoveRight(float val)
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

void ARCharacterClient::AddControllerYawInput(float dt)
{
	GetBase()->AddControllerYawInput(dt);
}

void ARCharacterClient::AddControllerPitchInput(float dt)
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

void ARCharacterClient::PrimaryAttack()
{
	TryRunProjectile(GetBase(), "PrimaryAttack");
}

void ARCharacterClient::DashAttack()
{
	TryRunProjectile(GetBase(), "DashAttack");
}

void ARCharacterClient::UltimateAttack()
{
	TryRunProjectile(GetBase(), "UltimateAttack");
}

void ARCharacterClient::SprintStart()
{
	GetBase()->GetActions()->StartAction("sprint", GetBase());
}

void ARCharacterClient::SprintEnd()
{
	GetBase()->GetActions()->StopAction("sprint", GetBase());
}

void ARCharacterClient::Jump()
{
	GetBase()->Jump();
}

void ARCharacterClient::PrimaryInteract()
{
	GetBase()->GetInteractionComponent()->PrimaryInteract();
}