#pragma once

#include <ARGame/Gameplay/Components/ARActionComponent.h>
#if AR_BUILD_SERVER
#include <ARGameServer/Gameplay/Actions/ActionServer.h>
#endif // AR_BUILD_SERVER


#include <CoreMinimal.h>
#include <GameplayTagContainer.h>
#include <UObject/Object.h>

#include "ARAction.generated.h"

class UARActionComponent;
struct FPredictedStartActionContext;

UCLASS(Blueprintable, BlueprintType)
class ARGAME_API UARAction : public UObject
{
	GENERATED_BODY()
	GENERATED_BASE_SERVER_SPLIT(UARAction, ar::server::ActionServer);

public:
	UARAction();
	
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
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& props) const override;
	// INTERFACE_END(UObject)

public:
	// We generate all the possible action callbacks so that they can be implement in blueprints.
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action|Client|Prediction")
	FPredictedStartActionContext OnClientPredictStart(AActor* instigator);
	virtual FPredictedStartActionContext OnClientPredictStart_Implementation(AActor* instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action|Client|Prediction")
	void OnFinalizeClientStartPrediction();
	virtual void OnFinalizeClientStartPrediction_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Action|Client|Prediction")
	void OnClientPredictStop(AActor* instigator);
	virtual void OnClientPredictStop_Implementation(AActor* instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action|Client|Prediction")
	void OnFinalizeClientStopPrediction();
	virtual void OnFinalizeClientStopPrediction_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Action|Client")
	void OnClientStart(AActor* instigator);
	virtual void OnClientStart_Implementation(AActor* instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action|Client")
	void OnClientStop(AActor* instigator);
	virtual void OnClientStop_Implementation(AActor* instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action|Server")
	void OnServerStart(AActor* instigator, const FPredictedStartActionContext& context);
	virtual void OnServerStart_Implementation(AActor* instigator,
											const FPredictedStartActionContext& context);
	UFUNCTION(BlueprintNativeEvent, Category = "Action|Server")
	void OnServerStop(AActor* instigator);
	virtual void OnServerStop_Implementation(AActor* instigator);

protected:
	UFUNCTION(BlueprintCallable, Category = "Action|Client|Prediction")
	void DispatchClientStopPrediction(AActor* instigator);

	UFUNCTION(BlueprintCallable, Category = "Action|Server")
	void DispatchServerStop(AActor* instigator);

public:
	UFUNCTION(Server, Reliable)
	void RPC_Server_Start(AActor* instigator, FPredictedStartActionContext context);

	UFUNCTION(Server, Reliable)
	void RPC_Server_Stop(AActor* instigator);

	UFUNCTION(NetMulticast, Reliable)
	void RPC_Multicast_ClientStart(AActor* instigator);

	UFUNCTION(NetMulticast, Reliable)
	void RPC_Multicast_ClientStop(AActor* instigator);

protected:
	// INTERFACE_BEGIN(UARAction)
	virtual bool CanStart_Implementation(AActor* instigator);

	// INTERFACE_END(UARAction)

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action|Config")
	FName ActionName;

	UPROPERTY(Replicated, BlueprintReadOnly, BlueprintReadOnly, Category = "Action|Runtime")
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