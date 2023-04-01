﻿#include <ARGameClient/Gameplay/Components/ARInteractionComponentClient.h>

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

void ARInteractionComponentClient::BeginPlay()
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

void ARInteractionComponentClient::EndPlay()
{
	GetBase()->GetWorld()->GetTimerManager().ClearTimer(FindFocusTimerHandle);
}

void ARInteractionComponentClient::NotifyIsLocalControlled()
{
	FTimerDelegate delegate;
	delegate.BindRaw(this, &ARInteractionComponentClient::FindBestInteractable);
	GetBase()->GetWorld()->GetTimerManager().SetTimer(FindFocusTimerHandle, std::move(delegate),
													  kFocusCheckPeriod, true);
}

void ARInteractionComponentClient::FindBestInteractable()
{
	NotNullPtr player_character = Cast<AARCharacter>(GetBase()->GetOwner());
	auto* interactable = GetBase()->QueryBestInteractable(player_character);
	ManageInteractableWidget(GetBase()->GetWidget(), interactable);
}