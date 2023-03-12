#include <ARGame/Gameplay/Components/ARInteractionComponent.h>

#include <ARGame/ARDebugCategories.h>
#include <ARGame/Gameplay/ARGameplayInterface.h>

#include <Kismet/KismetMathLibrary.h>
#include <Templates/NonNullPointer.h>

AR_REGISTER_DEBUG_CATEGORY(ARDebugCategories::INTERACTION, true,
						   "All the displays for player interactions");

// Sets default values for this component's properties
UARInteractionComponent::UARInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UARInteractionComponent::PrimaryInteract(const FVector& camera_target)
{
	constexpr float interaction_distance = 1000.0f;

	TNonNullPtr<AActor> owner = GetOwner();

	// Get the position/rotation of the character's eye.
	FVector eye_location;
	FRotator eye_rotation;
	owner->GetActorEyesViewPoint(eye_location, eye_rotation);

	// The trace we want it to point to the camera target rather than to look directly from the
	// character, because it gives some un-intuitive location.
	const FVector& start = eye_location;
	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(eye_location, camera_target);
	FVector end = eye_location + (rotation.Vector() * interaction_distance);

	constexpr float radius = 30.0f;
	FCollisionShape shape;
	shape.SetSphere(radius);

	FCollisionObjectQueryParams params = {};
	params.AddObjectTypesToQuery(ECC_WorldDynamic);

	bool interacted = false;

	TArray<FHitResult> out_hits;
	GetWorld()->SweepMultiByObjectType(out_hits, start, end, FQuat::Identity, params, shape);
	for (const auto& hit : out_hits)
	{
		if (AActor* hit_actor = hit.GetActor())
		{
			ARDebugDraw::Sphere(ARDebugCategories::INTERACTION, GetWorld(), hit.Location, radius,
								16, FColor::Yellow, 2, 2);

			if (hit_actor->Implements<UARInteractable>())
			{
				TNonNullPtr<APawn> pawn = Cast<APawn>(owner.Get());

				interacted = IARInteractable::Execute_Interact(hit_actor, pawn);
				if (interacted)
				{
					break;
				}

				break;
			}
		}
	}

	FColor color = interacted ? FColor::Green : FColor::Red;
	ARDebugDraw::Cylinder(ARDebugCategories::INTERACTION, GetWorld(), start, end, radius, 16, color,
						  2, 2);
}