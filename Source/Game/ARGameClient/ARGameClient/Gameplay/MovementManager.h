#pragma once
#include <ARBase/NotNullPtr.h>

class AARCharacter;

namespace ar
{
namespace client
{

class CharacterClient;

// MovementManager is helper class to take care of movement logic in order to de-clutter the main
// character class (CharacterClient).
class MovementManager 
{
public:
	AARCharacter* GetBase();
	
public:
	void SetupPlayerInput(NotNullPtr<CharacterClient> char_client,
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
	CharacterClient* CharacterClientSplit = nullptr;
};

} // namespace client
} // namespace ar

