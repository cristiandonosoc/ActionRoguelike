#pragma once

#include <ARBase/ClientServerSplit.h>
#include <ARGameClient/Gameplay/MovementManager.h>

class AARCharacter;

class UInputComponent;
class ARCharacterClient;

namespace ar_client
{

class ARGAMECLIENT_API CharacterClient
{
	GENERATED_CLIENT_SPLIT(AARCharacter, CharacterClient);

public:
	void SetupPlayerInput(NotNullPtr<UInputComponent> input);

	void NotifyControllerChanged();


private:
	MovementManager Movement;
};

} // namespace ar_client