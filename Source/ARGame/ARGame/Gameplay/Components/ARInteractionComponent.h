#pragma once

#include <ARBase/BuildDefines.h>
#include <Components/ActorComponent.h>
#include <CoreMinimal.h>

#include "ARInteractionComponent.generated.h"

class IARInteractable;
class UARActorAttachedWidget;

// UARInteractionComponent handles the interactivity part of the character.
//
// Currently it is not meant to be ticked directly, but rather it is meant to be called
// directly from the character. This is because the character holds much more information
// about the world, camera and where the player is looking at.
UCLASS()
class ARGAME_API UARInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	static constexpr float kInteractionDistance = 1000.0f;
	static constexpr float kInteractionRadius = 30.0f;

	static constexpr float kFocusCheckPeriod = 0.5f;

public:
	// Sets default values for this component's properties
	UARInteractionComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type reason) override;

	// Interact against the current best interactable.
	void PrimaryInteract();

#if AR_BUILD_CLIENT
	void NotifyIsLocalControlled();
#endif // AR_BUILD_CLIENT

protected:
	UFUNCTION()
	void FindBestInteractable();

	UFUNCTION(Server, Reliable)
	void Server_Interact();
	void Server_Interact_Implementation();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UARActorAttachedWidget> DefaultWidgetClass;

	UPROPERTY()
	TObjectPtr<UARActorAttachedWidget> Widget;

private:
#if AR_BUILD_CLIENT
	FTimerHandle FindFocusTimerHandle;
#endif // AR_BUILD_CLIENT
};