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

	bool GetIsClientStartPredicting() const { return IsClientStartPredicting; }
	bool GetIsClientStopPredicting() const { return IsClientStopPredicting; }

	bool GetIsRunning() const { return IsRunning; }

	const FGameplayTagContainer& GetGrantsTags() const { return GrantsTags; }
	const FGameplayTagContainer& GetBlockedTags() const { return BlockedTags; }

	UFUNCTION(BlueprintCallable, Category = "Action")
	UARActionComponent* GetOwningComponent() const;

public:
	// INTERFACE_BEGIN(UObject)
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual UWorld* GetWorld() const override;
	// INTERFACE_END(UObject)

public:
	// We generate all the possible action callbacks so that they can be implement in blueprints.
	
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action|Client|Prediction")
	void ClientPredictStart(AActor* instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action|Client|Prediction")
	void FinalizeClientStartPrediction();

	UFUNCTION(BlueprintNativeEvent, Category = "Action|Client|Prediction")
	void ClientPredictStop(AActor* instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action|Client|Prediction")
	void FinalizeClientStopPrediction();

	UFUNCTION(BlueprintNativeEvent, Category = "Action|Client")
	void ClientStart(AActor* instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action|Client")
	void ClientStop(AActor* instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action|Server")
	void ServerStart(AActor* instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action|Server")
	void ServerStop(AActor* instigator);


protected:
	UFUNCTION(BlueprintCallable, Category = "Action|Client|Prediction")
	void DispatchClientStopPrediction(AActor* instigator);

	UFUNCTION(BlueprintCallable, Category = "Action|Server")
	void DispatchServerStop(AActor* instigator);

protected:
	// INTERFACE_BEGIN(UARAction)
	virtual bool CanStart_Implementation(AActor* instigator);
	virtual void ClientPredictStart_Implementation(AActor* instigator);
	virtual void FinalizeClientStartPrediction_Implementation();
	virtual void ClientPredictStop_Implementation(AActor* instigator);
	virtual void FinalizeClientStopPrediction_Implementation();
	virtual void ClientStart_Implementation(AActor* instigator);
	virtual void ClientStop_Implementation(AActor* instigator);
	virtual void ServerStart_Implementation(AActor* instigator);
	virtual void ServerStop_Implementation(AActor* instigator);
	// INTERFACE_END(UARAction)

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action|Config")
	FName ActionName;

	UPROPERTY(BlueprintReadOnly, BlueprintReadOnly, Category = "Action|Runtime")
	bool IsRunning = false;

	// Whether this gets started as soon as someone adds the ability to the action component.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action|Config")
	bool AutoStarts = false;

	// Tags added to the owning actor when |Start| is called. Removed on |Stop|.
	UPROPERTY(EditDefaultsOnly, Category = "Action|Config")
	FGameplayTagContainer GrantsTags;

	// Action can only start if owning actor has none of this tags applied.
	// TODO(cdc): This seems like it really is a decision for the caller to have, not something an
	//            action should decide.
	UPROPERTY(EditDefaultsOnly, Category = "Action|Tags")
	FGameplayTagContainer BlockedTags;

	// CLIENT_PROPERTIES_BEGIN ---------------------------------------------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action|Config")
	bool IsClientOnly = false;

	UPROPERTY(BlueprintReadOnly, Category = "Action|Runtime")
	bool IsClientStartPredicting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Action|Runtime")
	bool IsClientStopPredicting = false;
	// CLIENT_PROPERTIES_END -----------------------------------------------------------------------
};