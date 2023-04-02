#include <ARGameClient/Gameplay/Components/InteractionComponentClient.h>

#include <ARGame/ARDebugCategories.h>
#include <ARGame/Gameplay/ARCharacter.h>
#include <ARGame/Gameplay/Components/ARInteractionComponent.h>
#include <ARGame/UI/ARActorAttachedWidget.h>

namespace ar
{
namespace client
{

namespace
{

void ManageInteractableWidget(NotNullPtr<UWorld> world, UARActorAttachedWidget* widget,
							  AActor* interactable)
{
	if (!widget)
	{
		ARDebugDraw::Text(ARDebugCategories::INTERACTION, world, "No default widget set!",
						  FColor::Red, 1);
		return;
	}

	// Manage the widget.
	if (interactable)
	{
		widget->AttachedTo = interactable;
		if (!widget->IsInViewport())
		{
			widget->AddToViewport();
		}
	}
	else
	{
		if (widget->IsInViewport())
		{
			widget->RemoveFromParent();
		}
	}
}


} // namespace

void InteractionComponentClient::BeginPlay()
{
	// Create the widget.
	if (GetBase()->GetDefaultWidgetClass())
	{
		check(!GetBase()->GetWidget());
		NotNullPtr widget = Cast<UARActorAttachedWidget>(
			CreateWidget(GetBase()->GetWorld(), GetBase()->GetDefaultWidgetClass().Get()));
		GetBase()->SetWidget(widget);
	}
}

void InteractionComponentClient::EndPlay()
{
	GetBase()->GetWorld()->GetTimerManager().ClearTimer(FindFocusTimerHandle);
}

void InteractionComponentClient::NotifyIsLocalControlled()
{
	FTimerDelegate delegate;
	delegate.BindRaw(this, &InteractionComponentClient::FindBestInteractable);
	GetBase()->GetWorld()->GetTimerManager().SetTimer(FindFocusTimerHandle, std::move(delegate),
													  kFocusCheckPeriod, true);
}

void InteractionComponentClient::FindBestInteractable()
{
	NotNullPtr player_character = Cast<AARCharacter>(GetOwner());
	auto* interactable = GetBase()->QueryBestInteractable(player_character);
	ManageInteractableWidget(GetWorld(), GetBase()->GetWidget(), interactable);
}

} // namespace client
} // namespace ar