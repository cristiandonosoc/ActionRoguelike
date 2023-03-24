#include <ARGame/Gameplay/Components/ARInteractionComponent.h>

#include <ARGame/ARDebugCategories.h>
#include <ARGame/Gameplay/ARCharacter.h>
#include <ARGame/Gameplay/ARGameplayInterface.h>
#include <ARGame/UI/ARActorAttachedWidget.h>
#include <Blueprint/UserWidget.h>

#include <Kismet/KismetMathLibrary.h>
#include <Templates/NonNullPointer.h>

AR_REGISTER_DEBUG_CATEGORY(ARDebugCategories::INTERACTION, false,
						   "All the displays for player interactions");

// Sets default values for this component's properties
UARInteractionComponent::UARInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UARInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Create the query timer.
	FTimerDelegate delegate;
	delegate.BindUFunction(this, "FindBestInteractable");
	GetWorld()->GetTimerManager().SetTimer(FindFocusTimerHandle, std::move(delegate),
										   kFocusCheckPeriod, true);

	// Create the widget.
	if (DefaultWidgetClass)
	{
		check(!Widget);
		Widget = Cast<UARActorAttachedWidget>(CreateWidget(GetWorld(), DefaultWidgetClass.Get()));
		check(Widget);
	}
}

void UARInteractionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	GetWorld()->GetTimerManager().ClearTimer(FindFocusTimerHandle);

	Super::EndPlay(EndPlayReason);
}

void UARInteractionComponent::TickComponent(float delta, ELevelTick tick_type,
											FActorComponentTickFunction* tick_function)
{
	Super::TickComponent(delta, tick_type, tick_function);
}

namespace
{

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
					ARDebugDraw::Sphere(ARDebugCategories::INTERACTION, world, hit.Location,
										UARInteractionComponent::kInteractionRadius, 16,
										FColor::Yellow, 2, 2);

					interactable = hit_actor;
				}
			}
		}
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

void UARInteractionComponent::FindBestInteractable()
{
	NotNullPtr player_character = Cast<AARCharacter>(GetOwner());
	FocusedInteractable = QueryBestInteractable(player_character);

	ManageInteractableWidget(Widget.Get(), FocusedInteractable.Get());
}

void UARInteractionComponent::PrimaryInteract()
{
	AActor* interactable = FocusedInteractable.Get();
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