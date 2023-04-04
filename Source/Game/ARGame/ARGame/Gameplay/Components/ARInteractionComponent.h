#pragma once

#include <ARBase/ClientServerSplit.h>

#if AR_BUILD_CLIENT
#include <ARGameClient/Gameplay/Components/InteractionComponentClient.h>
#endif // AR_BUILD_CLIENT

#if AR_BUILD_SERVER
#include <ARGameServer/Gameplay/Components/InteractionComponentServer.h>
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
	GENERATED_BASE_CLIENT_SPLIT(UARInteractionComponent, ar::client::InteractionComponentClient);
	GENERATED_BASE_SERVER_SPLIT(UARInteractionComponent, ar::server::InteractionComponentServer);

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

	static AActor* QueryBestInteractable(NotNullPtr<AARCharacter> owner);

	const auto& GetDefaultWidgetClass() const { return DefaultWidgetClass; }
	const TObjectPtr<UARActorAttachedWidget>& GetWidget() const { return Widget; }
	void SetWidget(NotNullPtr<UARActorAttachedWidget> widget) { Widget = widget; }

protected:
	UFUNCTION(Server, Reliable)
	void Server_Interact();
	void Server_Interact_Implementation();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UARActorAttachedWidget> DefaultWidgetClass;

	UPROPERTY()
	TObjectPtr<UARActorAttachedWidget> Widget;
};