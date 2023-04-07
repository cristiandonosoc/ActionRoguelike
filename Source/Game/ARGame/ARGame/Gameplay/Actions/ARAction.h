#pragma once

#include <CoreMinimal.h>
#include <GameplayTagContainer.h>
#include <UObject/Object.h>

#include "ARAction.generated.h"

class UARActionComponent;

UCLASS(Blueprintable, BlueprintType)
class ARGAME_API UARAction : public UObject
{
	GENERATED_BODY()

public:
	const FName& GetActionName() const { return ActionName; }
	bool GetIsClientOnly() const { return IsClientOnly; }
	bool GetAutoStarts() const { return AutoStarts; }
	bool GetIsRunning() const { return IsRunning; }
	bool GetIsClientPredicting() const { return IsClientPredicting; }

	UFUNCTION(BlueprintCallable, Category = "Action")
	UARActionComponent* GetOwningComponent() const;

public:
	// INTERFACE_BEGIN(UObject)
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual UWorld* GetWorld() const override;
	// INTERFACE_END(UObject)

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action|Client")
	void ClientPredictStart(AActor* instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action|Client")
	void ClientStart(AActor* instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action|Client")
	void ClientStop(AActor* instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action|Server")
	void ServerStart(AActor* instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action|Server")
	void ServerStop(AActor* instigator);

protected:
	// ClientPredictStop is protected because it will be called directly from ClientStart.
	UFUNCTION(BlueprintNativeEvent, Category = "Action|Client")
	void ClientPredictStop(AActor* instigator);

	// INTERFACE_BEGIN(UARAction)
	virtual bool CanStart_Implementation(AActor* instigator);
	virtual void ClientPredictStart_Implementation(AActor* instigator);
	virtual void ClientPredictStop_Implementation(AActor* instigator);
	virtual void ClientStart_Implementation(AActor* instigator);
	virtual void ClientStop_Implementation(AActor* instigator);
	virtual void ServerStart_Implementation(AActor* instigator);
	virtual void ServerStop_Implementation(AActor* instigator);
	// INTERFACE_END(UARAction)

protected:
	UPROPERTY(BlueprintReadOnly, BlueprintReadOnly, Category = "Action|Runtime")
	bool IsClientPredicting = false;

	UPROPERTY(BlueprintReadOnly, BlueprintReadOnly, Category = "Action|Runtime")
	bool IsRunning = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action|Config")
	FName ActionName;

	// Whether this gets started as soon as someone adds the ability to the action component.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action|Config")
	bool AutoStarts = false;

	UPROPERTY(EditDefaultsOnly, Category = "Action|Config")
	bool CanBeStartedMultipleTimes = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action|Config")
	bool IsClientOnly = false;

	// Tags added to the owning actor when |Start| is called. Removed on |Stop|.
	UPROPERTY(EditDefaultsOnly, Category = "Action|Config")
	FGameplayTagContainer GrantsTags;

	// Action can only start if owning actor has none of this tags applied.
	// TODO(cdc): This seems like it really is a decision for the caller to have, not something an
	//            action should decide.
	UPROPERTY(EditDefaultsOnly, Category = "Action|Tags")
	FGameplayTagContainer BlockedTags;
};