#include <ARGame/Gameplay/Components/ARInteractionComponent.h>

#include <ARGame/ARDebugCategories.h>
#include <ARGame/Gameplay/ARCharacter.h>
#include <ARGame/Gameplay/ARGameplayInterface.h>
#include <ARGame/UI/ARActorAttachedWidget.h>

#include <Blueprint/UserWidget.h>
#include <Components/BoxComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Templates/NonNullPointer.h>

AR_DECLARE_DEBUG_CATEGORY(INTERACTION, ARDebugCategories::INTERACTION, true,
						  "All the displays for player interactions");

// Sets default values for this component's properties
UARInteractionComponent::UARInteractionComponent()
{
	INIT_BASE_CLIENT_SERVER_SPLIT();
	PrimaryComponentTick.bCanEverTick = true;
}

void UARInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Create the widget.
	if (DefaultWidgetClass)
	{
		check(!Widget);
		Widget = Cast<UARActorAttachedWidget>(CreateWidget(GetWorld(), DefaultWidgetClass.Get()));
		check(Widget);
	}
}

void UARInteractionComponent::EndPlay(const EEndPlayReason::Type reason)
{
	CLIENT_CALL(EndPlay);
	Super::EndPlay(reason);
}

AActor* UARInteractionComponent::QueryBestInteractable(NotNullPtr<AARCharacter> owner)
{
	TArray<AActor*> actors;
	owner->GetInteractionBox()->GetOverlappingActors(actors);

	if (actors.IsEmpty())
	{
		return nullptr;
	}

	// Get the closest interactable.
	// We could collect them all if we want a more clever criteria to select the best interactable.
	FVector owner_location = owner->GetActorLocation();
	double min_dist_squared = std::numeric_limits<double>::max();
	AActor* interactable = nullptr;

	for (AActor* actor : actors)
	{
		if (!actor->Implements<UARInteractable>())
		{
			continue;
		}

		// Now we see the distance and see if it's the closest.
		double dist = FVector::DistSquared(owner_location, actor->GetActorLocation());
		if (dist > min_dist_squared)
		{
			continue;
		}

		min_dist_squared = dist;
		interactable = actor;
	}

	if (!interactable)
	{
		return nullptr;
	}

	return interactable;
}

void UARInteractionComponent::PrimaryInteract()
{
	Server_Interact();
}


void UARInteractionComponent::Server_Interact_Implementation()
{
	SERVER_CALL(Interact);
}