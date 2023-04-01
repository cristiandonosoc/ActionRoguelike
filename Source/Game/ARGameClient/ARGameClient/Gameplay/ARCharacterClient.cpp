#include <ARGameClient/Gameplay/ARCharacterClient.h>

#include <ARGame/Gameplay/ARCharacter.h>
#include <ARGame/Gameplay/Components/ARInteractionComponent.h>

namespace ar_client
{

void CharacterClient::SetupPlayerInput(NotNullPtr<UInputComponent> input)
{
	Movement.SetupPlayerInput(this, input);
}

void CharacterClient::NotifyControllerChanged()
{
	// We only care if we're locally controller, to tell the interaction component that it can
	// start tracking interactables (for player feedback).
	if (GetBase()->IsLocallyControlled())
	{
		GetBase()->GetInteractionComponent()->GetClientSplit()->NotifyIsLocalControlled();
	}
}

} // namespace ar_client