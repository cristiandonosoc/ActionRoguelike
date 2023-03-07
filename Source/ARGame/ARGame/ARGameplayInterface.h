// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <UObject/Interface.h>

#include "ARGameplayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UARGameplayInterface : public UInterface
{
	GENERATED_BODY()
};

class ARGAME_API IARGameplayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Interact attempts to interact with the gameplay object.
	// Returns true if the interaction is valid. False means that the object is not available to be
	// interacted and users should continue looking for another interaction.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool Interact(APawn* interactor);
};
