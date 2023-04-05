#pragma once

#include <ARBase/ClientServerSplit.h>
#include <ARGameClient/Gameplay/MovementManager.h>


struct FOnHealthChangedPayload;
class AARCharacter;

class UInputComponent;
class ARCharacterClient;

namespace ar
{
namespace client
{

class ARGAMECLIENT_API CharacterClient
{
	GENERATED_LEAF_CLIENT_SPLIT(AARCharacter, CharacterClient);

public:
	void SetupPlayerInput(NotNullPtr<UInputComponent> input);

	void NotifyControllerChanged();

	void OnHealthChanged(const FOnHealthChangedPayload& payload);

private:
	MovementManager Movement;
};

} // namespace client
} // namespace ar