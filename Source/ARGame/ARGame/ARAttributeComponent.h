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

	static constexpr uint8 FLAG_KILLED = 0b00000001;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AActor> Instigator;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UARAttributeComponent> Target;

	UPROPERTY(BlueprintReadOnly)
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly)
	float NewHealth;

	// OriginalDelta is the original change requested.
	// Depending on the current value, it could be different.
	// See |ActualDelta| for the real change made by the event.
	UPROPERTY(BlueprintReadOnly)
	float OriginalDelta;

	// ActualDelta is the real change to the value performed by the event.
	UPROPERTY(BlueprintReadOnly)
	float ActualDelta;

	UPROPERTY(BlueprintReadOnly)
	uint8 Flags;

	// Convenient getters.
	bool Killed() const { return Flags & FLAG_KILLED; }
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
	// |instigator| can be null.
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* instigator, float delta);

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const { return Health > 0.0f; }

	UFUNCTION(BlueprintCallable)
	float LifeRatio() const { return Health / MaxHealth; }

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
