﻿#pragma once

#include <ARBase/ClientServerSplit.h>
#include <ARBase/NotNullPtr.h>

#if AR_BUILD_CLIENT
#include <ARGameClient/Gameplay/Components/AttributeComponentClient.h>
#endif // AR_BUILD_CLIENT
#if AR_BUILD_SERVER
#include <ARGameServer/Gameplay/Components/AttributeComponentServer.h>
#endif // AR_BUILD_SERVER

#include <Components/ActorComponent.h>
#include <CoreMinimal.h>

#include "ARAttributeComponent.generated.h"

class UARAttributeComponent;

USTRUCT(BlueprintType)
struct FOnHealthChangedPayload
{
	GENERATED_BODY();

	static constexpr uint8 FLAG_KILLED = 0b00000001;

public:
	// Convenient flags getters.
	bool Killed() const { return Flags & FLAG_KILLED; }
	void SetKilled() { Flags |= FLAG_KILLED; }

public:
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AActor> Instigator;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UARAttributeComponent> Target;

	UPROPERTY(BlueprintReadOnly)
	float MaxHealth = 0;

	UPROPERTY(BlueprintReadOnly)
	float NewHealth = 0;

	// OriginalDelta is the original change requested.
	// Depending on the current value, it could be different.
	// See |ActualDelta| for the real change made by the event.
	UPROPERTY(BlueprintReadOnly)
	float OriginalDelta = 0;

	// ActualDelta is the real change to the value performed by the event.
	UPROPERTY(BlueprintReadOnly)
	float ActualDelta = 0;

	UPROPERTY(BlueprintReadOnly)
	uint8 Flags = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, const FOnHealthChangedPayload&,
											payload);

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ARGAME_API UARAttributeComponent : public UActorComponent
{
	GENERATED_BODY()
	GENERATED_BASE_CLIENT_SPLIT(UARAttributeComponent, ar::client::AttributeComponentClient);
	GENERATED_BASE_SERVER_SPLIT(UARAttributeComponent, ar::server::AttributeComponentServer);

public:
	// Helper statics.
	// NOTE: These assume that the actor will have the attribute component, so it will assert on it.
	static bool IsActorAlive(NotNullPtr<AActor> actor);
	static UARAttributeComponent& GetAttributes(NotNullPtr<AActor> actor);

public:
	UARAttributeComponent();

public:
	float GetHealth() const { return Health; }
	void SetHealth(float health) { Health = health; }
	
	float GetMaxHealth() const { return MaxHealth; }
	float GetCredits() const { return Credits; }

	const FOnHealthChanged& GetOnHealthChangedDelegate() const { return OnHealthChanged; }

public:
	// INTERFACE_BEGIN(UActorComponent)
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& props) const override;
	// INTERFACE_END(UActorComponent)

	// Checks whether the health change attempt would apply.
	// This is validated by |ApplyHealthChanged| as well, but it can be useful for certain agents to
	// query this to see if they need to perform the action (eg. Health pack).
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool WouldHealthChangeApply(float delta) const;

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const { return Health > 0.0f; }

	UFUNCTION(BlueprintCallable)
	float LifeRatio() const { return Health / MaxHealth; }

	// Returns whether the change was applied.
	// |instigator| can be null.
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool Server_ApplyHealthChange(AActor* instigator, float delta);

protected:
	UFUNCTION()
	void OnRep_Health(float old_health);
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

protected:
	UPROPERTY(ReplicatedUsing = "OnRep_Health", EditDefaultsOnly, BlueprintReadOnly,
			  Category = "Attributes")
	float Health = 200;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float MaxHealth = 200;

	// How many credits the owner gives when killed.
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	int32 Credits = 0;
};