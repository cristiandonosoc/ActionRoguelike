// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <ARGame/Gameplay/ARGameplayInterface.h>

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

#include "ARItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ARGAME_API AARItemChest : public AActor, public IARInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AARItemChest();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& props) const override;

	// INTERFACE_BEGIN(IARGameplayInterface)
	virtual bool CanInteract_Implementation(APawn* interactor) override;
	virtual bool Interact_Implementation(APawn* interactor) override;
	// INTERFACE_END(IARGameplayInterface)

protected:
	UFUNCTION()
	void OnRep_IsOpenedChanged();

private:
	void ToggleChestAnimation();

public:
	UPROPERTY(EditAnywhere)
	float TargetPitch = 110.0f;

protected:
	UPROPERTY(ReplicatedUsing = "OnRep_IsOpenedChanged")
	bool IsOpened = false;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* LidMesh;
};