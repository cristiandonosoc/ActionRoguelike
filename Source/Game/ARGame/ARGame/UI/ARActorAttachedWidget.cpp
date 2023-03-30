#include <ARGame/UI/ARActorAttachedWidget.h>

#include <ARBase/NotNullPtr.h>

#include <Blueprint/WidgetLayoutLibrary.h>
#include <Components/SizeBox.h>

namespace
{

NotNullPtr<APlayerController> GetLocalPlayerController(NotNullPtr<UWorld> world)
{
	for (FConstPlayerControllerIterator Iterator = world->GetPlayerControllerIterator(); Iterator;
		 ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (PlayerController && PlayerController->IsLocalController())
		{
			return PlayerController;
		}
	}

	check(false);
	APlayerController* is_null = nullptr;
	return is_null;
}

} // namespace


void UARActorAttachedWidget::NativeTick(const FGeometry& geometry, float delta)
{
	Super::NativeTick(geometry, delta);

	if (!AttachedTo)
	{
		return;
	}

	if (!ParentSizeBox)
	{
		return;
	}

	auto player_controller = GetLocalPlayerController(GetWorld());

	// Calculate the position of the widget.
	FVector location = AttachedTo->GetActorLocation() + TranslationOffset;
	FVector2D out_location;
	if (!UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(player_controller, location,
																	out_location, true))
	{
		return;
	}

	ParentSizeBox->SetRenderTranslation(out_location + OnScreenTranslateOffset);
	ParentSizeBox->SetRenderScale(OnScreenScale);
}