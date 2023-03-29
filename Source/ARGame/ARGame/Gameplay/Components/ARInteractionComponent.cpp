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
#if AR_BUILD_CLIENT
	GetWorld()->GetTimerManager().ClearTimer(FindFocusTimerHandle);
#endif // AR_BUILD_CLIENT
	Super::EndPlay(reason);
}

namespace
{

#ifdef THIS_IS_DEPRECATED
AActor* QueryBestInteractable(NotNullPtr<AARCharacter> owner)
{
	NotNullPtr<UWorld> world = owner->GetWorld();

	// Get the position/rotation of the character's eye.
	FVector eye_location;
	FRotator eye_rotation;
	owner->GetActorEyesViewPoint(eye_location, eye_rotation);

	// The trace we want it to point to the camera target rather than to look directly from the
	// character, because it gives some un-intuitive location.
	const FVector& start = eye_location;
	FRotator rotation =
		UKismetMathLibrary::FindLookAtRotation(eye_location, owner->GetCameraTarget());
	FVector end =
		eye_location + (rotation.Vector() * UARInteractionComponent::kInteractionDistance);

	FCollisionShape shape;
	shape.SetSphere(UARInteractionComponent::kInteractionRadius);

	FCollisionObjectQueryParams params = {};
	params.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* interactable = nullptr;

	TArray<FHitResult> out_hits;
	world->SweepMultiByObjectType(out_hits, start, end, FQuat::Identity, params, shape);
	for (const auto& hit : out_hits)
	{
		if (AActor* hit_actor = hit.GetActor())
		{
			if (hit_actor->Implements<UARInteractable>())
			{
				TNonNullPtr<APawn> pawn = Cast<APawn>(owner.Get());

				if (IARInteractable::Execute_CanInteract(hit_actor, pawn))
				{
					interactable = hit_actor;
					ARDebugDraw::Sphere(ARDebugCategories::INTERACTION, world, hit.Location,
										UARInteractionComponent::kInteractionRadius, 16,
										FColor::Yellow, 2, 2);
					if (owner->HasAuthority())
					{
						ARDebugDraw::Text(
							ARDebugCategories::INTERACTION,
							FString::Printf(TEXT("Found interactable: %s (owner: %s)"),
											*GetNameSafe(interactable), *GetNameSafe(owner)),
							FColor::Black, 0.2f);
					}
				}
			}
		}
	}

	FColor color = interactable ? FColor::Green : FColor::Red;
	ARDebugDraw::Cylinder(ARDebugCategories::ALWAYS, world, start, end,
						  UARInteractionComponent::kInteractionRadius, 16, color, 2, 1);

	return interactable;
}
#endif // THIS_IS_DEPRECATED

AActor* QueryBestInteractable(NotNullPtr<AARCharacter> owner)
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

#if AR_BUILD_CLIENT

void UARInteractionComponent::NotifyIsLocalControlled()
{
	FTimerDelegate delegate;
	delegate.BindUFunction(this, "FindBestInteractable");
	GetWorld()->GetTimerManager().SetTimer(FindFocusTimerHandle, std::move(delegate),
										   kFocusCheckPeriod, true);
}

#endif // AR_BUILD_CLIENT

void UARInteractionComponent::PrimaryInteract()
{
	Server_Interact();
}

void UARInteractionComponent::FindBestInteractable()
{
	NotNullPtr player_character = Cast<AARCharacter>(GetOwner());
	auto* interactable = QueryBestInteractable(player_character);
	ManageInteractableWidget(Widget.Get(), interactable);
}

void UARInteractionComponent::Server_Interact_Implementation()
{
	NotNullPtr player_character = Cast<AARCharacter>(GetOwner());
	AActor* interactable = QueryBestInteractable(player_character);
	if (!interactable)
	{
		ARDebugDraw::Text(ARDebugCategories::INTERACTION, "No current focused interactable",
						  FColor::Orange, 3);
		return;
	}

	check(interactable->Implements<UARInteractable>());

	NotNullPtr pawn = Cast<APawn>(GetOwner());
	if (!IARInteractable::Execute_CanInteract(interactable, pawn))
	{
		return;
	}

	IARInteractable::Execute_Interact(interactable, pawn);
}