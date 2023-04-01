#pragma once

#include <ARBase/ClientServerSplit.h>

class AARCharacter;

class UInputComponent;
class ARCharacterClient;

namespace ARCharacterClient_Private
{

// MovementHelperManager is helper class to take care of movement logic in order to de-clutter
// ARCharacterClient.
class MovementHelperManager
{
public:
	AARCharacter* GetBase();
	
public:
	void SetupPlayerInput(NotNullPtr<ARCharacterClient> char_client,
						  NotNullPtr<UInputComponent> input);

private:
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

private:
	ARCharacterClient* ClientSplit = nullptr;
};

} // namespace ARCharacterClient_Private


class ARGAMECLIENT_API ARCharacterClient
{
	GENERATED_CLIENT_SPLIT(AARCharacter, ARCharacterClient);

public:
	void SetupPlayerInput(NotNullPtr<UInputComponent> input);

	void NotifyControllerChanged();


private:
	ARCharacterClient_Private::MovementHelperManager MovementHelper;
};