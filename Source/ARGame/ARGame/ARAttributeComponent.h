// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "ARAttributeComponent.generated.h"

class UARAttributeComponent;

USTRUCT(BlueprintType)
struct FOnHealthChangedPayload
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AActor> Instigator;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UARAttributeComponent> Target;

	UPROPERTY(BlueprintReadOnly)
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly)
	float NewHealth;

	UPROPERTY(BlueprintReadOnly)
	float Delta;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, const FOnHealthChangedPayload&, payload);

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ARGAME_API UARAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UARAttributeComponent();

	// Checks whether the health change attempt would apply.
	// This is validated by |ApplyHealthChanged| as well, but it can be useful for certain agents to
	// query this to see if they need to perform the action (eg. Health pack).
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool WouldHealthChangeApply(float delta) const;

	// Returns whether the change was applied.
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(float delta);

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const { return Health > 0.0f; }

	UFUNCTION(BlueprintCallable)
	float LifeRatio() const { return Health / MaxHealth; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Health = 200;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float MaxHealth = 200;

	// HealthMax, Stamina, Strength
};
