#include <ARGameClient/AI/AICharacterClient.h>

#include <ARGame/AI/ARAICharacter.h>
#include <ARGame/Gameplay/Components/ARAttributeComponent.h>
#include <ARGame/UI/ARWidgetManager.h>
#include <ARGame/UI/Widgets/ARDamagePopupWidget.h>
#include <ARGame/UI/Widgets/AREnemyHealthBarWidget.h>

namespace ar
{
namespace client
{

void AICharacterClient::OnHealthChanged(const FOnHealthChangedPayload& payload)
{
	if (!HealthBarWidget)
	{
		HealthBarWidget = UARWidgetManager::CreateEnemyHealthBarWidget(
			GetBase(), GetBase()->Attributes->GetHealth(), GetBase()->Attributes->GetMaxHealth());
		if (HealthBarWidget)
		{
			// TODO(cdc): Make this translation configurable.
			HealthBarWidget->TranslationOffset = FVector(0, 0, 75);
			HealthBarWidget->AddToViewport();
		}
	}
	else
	{
		HealthBarWidget->CurrentHealth = GetBase()->Attributes->GetHealth();
		HealthBarWidget->MaxHealth = GetBase()->Attributes->GetMaxHealth();
	}

	if (payload.ActualDelta < 0.0f)
	{
		if (auto* mesh = GetBase()->GetMesh())
		{
			mesh->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
		}

		auto* popup = UARWidgetManager::CreateDamagePopupWidget(GetBase(), payload.ActualDelta);
		check(popup);
		popup->AddToViewport();
	}

	if (payload.Killed())
	{
		// Rag-doll.
		// We make all bones simulate physics.
		if (auto* mesh = GetBase()->GetMesh())
		{
			mesh->SetAllBodiesSimulatePhysics(true);
			mesh->SetCollisionProfileName(TEXT("Ragdoll"));
		}

		if (HealthBarWidget)
		{
			HealthBarWidget->RemoveFromParent();
		}
	}
}


} // namespace client
} // namespace ar