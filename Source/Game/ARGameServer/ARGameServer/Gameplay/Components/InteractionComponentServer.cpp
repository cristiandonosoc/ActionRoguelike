#include <ARGameServer/Gameplay/Components/InteractionComponentServer.h>

#include <ARGame/ARDebugCategories.h>
#include <ARGame/Gameplay/ARCharacter.h>
#include <ARGame/Gameplay/Components/ARInteractionComponent.h>

namespace ar
{
namespace server
{

void InteractionComponentServer::Interact()
{
	NotNullPtr player_character = Cast<AARCharacter>(GetBase()->GetOwner());
	AActor* interactable = GetBase()->QueryBestInteractable(player_character);
	if (!interactable)
	{
		ARDebugDraw::Text(ARDebugCategories::INTERACTION, "No current focused interactable",
						  FColor::Orange, 3);
		return;
	}

	check(interactable->Implements<UARInteractable>());

	NotNullPtr pawn = Cast<APawn>(player_character.Get());
	if (!IARInteractable::Execute_CanInteract(interactable, pawn))
	{
		return;
	}

	IARInteractable::Execute_Interact(interactable, pawn);
}

} // namespace server
} // namespace ar
