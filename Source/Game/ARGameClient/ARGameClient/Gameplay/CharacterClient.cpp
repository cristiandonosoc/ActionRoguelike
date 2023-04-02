#include <ARGameClient/Gameplay/CharacterClient.h>

#include <ARGame/Gameplay/ARCharacter.h>
#include <ARGame/Gameplay/Components/ARAttributeComponent.h>
#include <ARGame/Gameplay/Components/ARInteractionComponent.h>

namespace ar
{
namespace client
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

void CharacterClient::OnHealthChanged(const FOnHealthChangedPayload& payload)
{
	NotNullPtr<AARCharacter> character = GetBase();

	if (payload.Killed())
	{
		character->DisableInput(Cast<APlayerController>(character->GetController()));
	}

	// Attempt to set the flash effect, camera shake, etc.
	if (payload.ActualDelta < 0.0f)
	{
		if (USkeletalMeshComponent* mesh = character->GetMesh())
		{
			mesh->SetScalarParameterValueOnMaterials("TimeToHit",
													 character->GetWorld()->TimeSeconds);
		}

		// Only on local controller, we want to move the camera.
		// TODO(cdc): Better handling of local character vs remote character.
		//		      Likely something like a local split.
		if (character->IsLocallyControlled())
		{
			if (auto camera_shake = character->GetCameraShake())
			{
				character->GetLocalViewingPlayerController()
					->PlayerCameraManager->PlayWorldCameraShake(
						character->GetWorld(), camera_shake.Get(), character->GetActorLocation(),
						0.0f, 10000.0f, 0.1f);
			}
		}
	}
}

} // namespace client
} // namespace ar