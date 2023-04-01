#pragma once

#include <ARBase/ClientServerSplit.h>

class AARCharacter;

class UInputComponent;

class ARGAMECLIENT_API ARCharacterClient
{
	GENERATED_CLIENT_SPLIT(AARCharacter, ARCharacterClient);

public:
	void SetupPlayerInput(NotNullPtr<UInputComponent> input);

	void NotifyControllerChanged();

	// Movement.
	void MoveForward(float val);
	void MoveRight(float val);
	void AddControllerYawInput(float dt);
	void AddControllerPitchInput(float dt);

	void PrimaryAttack();
	void DashAttack();
	void UltimateAttack();

	void SprintStart();
	void SprintEnd();
	void Jump();

	void PrimaryInteract();
};