// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <UObject/Interface.h>

#include "ARGameplayInterface.generated.h"

// Interactable ------------------------------------------------------------------------------------
//
// This interface is for all the actors that can be interacted with.
// This will be used by systems to query for certain elements that can be interacted with.

UINTERFACE(MinimalAPI, Blueprintable)
class UARInteractable : public UInterface
{
	GENERATED_BODY()
};

class ARGAME_API IARInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement
	// this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanInteract(APawn* interactor);
	
	// Interact attempts to interact with the gameplay object.
	// Returns true if the interaction is valid. False means that the object is not available to be
	// interacted and users should continue looking for another interaction.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool Interact(APawn* interactor);
};

// CreditHolder ------------------------------------------------------------------------------------
//
// This is an interface for all those actors that can hold/use credits.


UINTERFACE(MinimalAPI, Blueprintable)
class UARCreditHolder : public UInterface
{
	GENERATED_BODY()
};

class ARGAME_API IARCreditHolder
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetCurrentCredits();

	// Add changes the amount of credits to the player.
	// Returns the amount of credits after the change.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 AddCredits(int32 credits);

	// PayCredits attempts to pay a certain amount of credits.
	// Returns true if it could pay that amount.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool PayCredits(int32 price);
};