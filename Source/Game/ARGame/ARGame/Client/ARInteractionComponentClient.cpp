#include <ARGame/Client/ARInteractionComponentClient.h>
#ifdef AR_BUILD_CLIENT

#include <ARGame/ARDebugCategories.h>
#include <ARGame/Gameplay/ARCharacter.h>
#include <ARGame/Gameplay/Components/ARInteractionComponent.h>
#include <ARGame/UI/ARActorAttachedWidget.h>

namespace
{

void ManageInteractableWidget(UARActorAttachedWidget* widget, AActor* interactable)
{
	if (!widget)
	{
		ARDebugDraw::Text(ARDebugCategories::INTERACTION, "No default widget set!", FColor::Red, 1);
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

void ARInteractionComponentClient::NotifyIsLocalControlled()
{
	FTimerDelegate delegate;
	delegate.BindRaw(this, &ARInteractionComponentClient::FindBestInteractable);
	Base->GetWorld()->GetTimerManager().SetTimer(FindFocusTimerHandle, std::move(delegate),
												 kFocusCheckPeriod, true);
}

void ARInteractionComponentClient::FindBestInteractable()
{
	NotNullPtr player_character = Cast<AARCharacter>(Base->GetOwner());
	auto* interactable = Base->QueryBestInteractable(player_character);
	ManageInteractableWidget(Base->Widget.Get(), interactable);
}
void ARInteractionComponentClient::EndPlay()
{
	Base->GetWorld()->GetTimerManager().ClearTimer(FindFocusTimerHandle);
}

#endif // AR_BUILD_CLIENT