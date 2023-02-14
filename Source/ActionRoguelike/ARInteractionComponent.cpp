// Fill out your copyright notice in the Description page of Project Settings.


#include "ARInteractionComponent.h"

#include "ARGameplayInterface.h"

// Sets default values for this component's properties
UARInteractionComponent::UARInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You
	// can turn these features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

namespace ARInteractionComponent_Private
{

void VisualizeLineTrace(const UARInteractionComponent& Component, const FVector& Start,
						const FVector& End)
{
	UWorld* World = Component.GetWorld();
	DrawDebugLine(World, Start, End, FColor::Purple, false, 1, 0, 1);
}


void InteractByLineTrace(const UARInteractionComponent& Component)
{
	FCollisionObjectQueryParams Params = {};
	Params.AddObjectTypesToQuery(ECC_WorldDynamic);

	FVector EyeLocation;
	FRotator EyeRotation;

	AActor* Owner = Component.GetOwner();
	check(Owner);

	Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector Start = EyeLocation;
	FVector End = EyeLocation + (EyeRotation.Vector() * 1000.0f);

	ARInteractionComponent_Private::VisualizeLineTrace(Component, Start, End);

	FHitResult Hit;
	if (Component.GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, Params))
	{
		UE_LOG(LogTemp, Log, TEXT("Interacted!"));

		AActor* HitActor = Hit.GetActor();
		check(HitActor);
		if (HitActor->Implements<UARGameplayInterface>())
		{
			APawn* Pawn = Cast<APawn>(Owner);
			check(Pawn);

			IARGameplayInterface::Execute_Interact(HitActor, Pawn);
		}
	}
}

} // namespace ARInteractionComponent_Private


void UARInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams Params = {};
	Params.AddObjectTypesToQuery(ECC_WorldDynamic);

	FVector EyeLocation;
	FRotator EyeRotation;

	AActor* Owner = GetOwner();
	check(Owner);

	Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector Start = EyeLocation;
	FVector End = EyeLocation + (EyeRotation.Vector() * 1000.0f);

	constexpr float Radius = 30.0f;
	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	TArray<FHitResult> Hits;
	GetWorld()->SweepMultiByObjectType(Hits, Start, End, FQuat::Identity, Params, Shape);
	bool Interacted = false;
	for (const auto& Hit : Hits)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			DrawDebugSphere(GetWorld(), Hit.Location, Radius, 16, FColor::Yellow, false, 2, 0, 2);

			if (HitActor->Implements<UARGameplayInterface>())
			{
				APawn* Pawn = Cast<APawn>(Owner);
				check(Pawn);

				IARGameplayInterface::Execute_Interact(HitActor, Pawn);
				Interacted = true;
				break;
			}
		}
	}

	FColor Color = Interacted ? FColor::Green : FColor::Red;
	DrawDebugCylinder(GetWorld(), Start, End, Radius, 16, Color, false, 2, 0, 2);
}


// Called when the game starts
void UARInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UARInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
											FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}