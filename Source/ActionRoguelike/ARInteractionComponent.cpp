// Fill out your copyright notice in the Description page of Project Settings.


#include "ARInteractionComponent.h"

#include "ARGameplayInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Templates/NonNullPointer.h"

// Sets default values for this component's properties
UARInteractionComponent::UARInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You
	// can turn these features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
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
			DrawDebugSphere(GetWorld(), hit.Location, radius, 16, FColor::Yellow, false, 2, 0, 2);

			if (hit_actor->Implements<UARGameplayInterface>())
			{
				TNonNullPtr<APawn> pawn = Cast<APawn>(owner.Get());

				IARGameplayInterface::Execute_Interact(hit_actor, pawn);
				interacted = true;
				break;
			}
		}
	}

	FColor color = interacted ? FColor::Green : FColor::Red;
	DrawDebugCylinder(GetWorld(), start, end, radius, 16, color, false, 2, 0, 2);
}


// Called when the game starts
void UARInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}