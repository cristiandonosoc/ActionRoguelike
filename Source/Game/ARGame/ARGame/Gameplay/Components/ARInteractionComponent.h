#pragma once

#include <ARBase/BuildDefines.h>
#include <ARBase/ClientServerSplit.h>

#ifdef AR_BUILD_CLIENT
#include <ARGame/Client/ARInteractionComponentClient.h>
#endif // AR_BUILD_CLIENT

#ifdef AR_BUILD_SERVER
#include <ARGame/Server/ARInteractionComponentServer.h>
#endif // AR_BUILD_SERVER

#include <Components/ActorComponent.h>
#include <CoreMinimal.h>

#include "ARInteractionComponent.generated.h"

class AARCharacter;
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
	GENERATED_BASE_CLIENT_SERVER_SPLIT(UARInteractionComponent, ARInteractionComponentClient,
									   ARInteractionComponentServer);

public:
	static constexpr float kInteractionDistance = 1000.0f;
	static constexpr float kInteractionRadius = 30.0f;

public:
	// Sets default values for this component's properties
	UARInteractionComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type reason) override;

	// Interact against the current best interactable.
	void PrimaryInteract();

protected:
	UFUNCTION(Server, Reliable)
	void Server_Interact();
	void Server_Interact_Implementation();

private:
	static AActor* QueryBestInteractable(NotNullPtr<AARCharacter> owner);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UARActorAttachedWidget> DefaultWidgetClass;

	UPROPERTY()
	TObjectPtr<UARActorAttachedWidget> Widget;
};