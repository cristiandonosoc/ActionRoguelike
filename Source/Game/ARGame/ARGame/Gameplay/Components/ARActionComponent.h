#pragma once

#include <ARBase/ClientServerSplit.h>
#if AR_BUILD_CLIENT
#include <ARGameClient/Gameplay/Components/ActionComponentClient.h>
#endif // AR_BUILD_CLIENT
#if AR_BUILD_SERVER
#include <ARGameServer/Gameplay/Components/ActionComponentServer.h>
#endif // AR_BUILD_SERVER

#include <Components/ActorComponent.h>
#include <CoreMinimal.h>
#include <GameplayTagContainer.h>

#include "ARActionComponent.generated.h"

class UARAction;

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class ARGAME_API UARActionComponent : public UActorComponent
{
	GENERATED_BODY()
	GENERATED_BASE_CLIENT_SPLIT(UARActionComponent, ar::client::ActionComponentClient);
	GENERATED_BASE_SERVER_SPLIT(UARActionComponent, ar::server::ActionComponentServer);

	// To keep the public API clean, we allow access to the client.
#if AR_BUILD_CLIENT
	friend class ar::client::ActionComponentClient;
#endif // AR_BUILD_CLIENT

public:
	// Sets default values for this component's properties
	UARActionComponent();

public:
	FGameplayTagContainer& GetActiveGameplayTags() { return ActiveGameplayTags; }
	const TArray<TObjectPtr<UARAction>>& GetActions() const { return Actions; }

	const TArray<TSoftClassPtr<UARAction>>& GetServer_DefaultActions() const;

public:
	// INTERFACE_BEGIN(UActorComponent)
	virtual void BeginPlay() override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& props) const;
	virtual bool ReplicateSubobjects(UActorChannel* channel, FOutBunch* bunch,
									 FReplicationFlags* rep_flags) override;
	virtual void TickComponent(float delta, ELevelTick tick_type,
							   FActorComponentTickFunction* tick_function) override;
	// INTERFACE_END(UActorComponent)

public:
	UFUNCTION(BlueprintCallable, Category = "Action")
	void AddAction(TSubclassOf<UARAction> action_class, AActor* instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	void RemoveAction(const FName& name);

	UFUNCTION(BlueprintCallable, Category = "Action")
	UARAction* FindAction(const FName& name) const;

	UFUNCTION(BlueprintCallable, Category = "Action")
	void ClientPredictStartAction(const FName& name, AActor* instigator);

	UFUNCTION(BlueprintCallable, Category = "Action|Server")
	void ServerStartAction(const FName& name, AActor* instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	void StopAction(const FName& name, AActor* instigator);

protected:
	UFUNCTION(Server, Reliable)
	void RPC_Server_StartAction(UARAction* action, AActor* instigator);

	UFUNCTION(Server, Reliable)
	void RPC_Server_StopAction(UARAction* action, AActor* instigator);

	UFUNCTION(NetMulticast, Reliable)
	void RPC_Multicast_StartAction(UARAction* action, AActor* instigator);

	UFUNCTION(NetMulticast, Reliable)
	void RPC_Multicast_StopAction(UARAction* action, AActor* instigator);

	// INTERFACE_BEGIN(UARActionComponent)
	UFUNCTION()
	void OnRep_Actions(TArray<UARAction*> old_actions);
	void RPC_Server_StartAction_Implementation(UARAction* action, AActor* instigator);
	void RPC_Server_StopAction_Implementation(UARAction* action, AActor* instigator);
	void RPC_Multicast_StartAction_Implementation(UARAction* action, AActor* instigator);
	void RPC_Multicast_StopAction_Implementation(UARAction* action, AActor* instigator);
	// INTERFACE_END(UARActionComponent)

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UPROPERTY(ReplicatedUsing = "OnRep_Actions", EditAnywhere)
	TArray<TObjectPtr<UARAction>> Actions;

	// SERVER_PROPERTIES_BEGIN ---------------------------------------------------------------------
	// Default actions will be created upon startup.
	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSoftClassPtr<UARAction>> Server_DefaultActions;
	// SERVER_PROPERTIES_END -----------------------------------------------------------------------
};