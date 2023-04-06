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
	virtual bool ReplicateSubobjects(UActorChannel* channel, FOutBunch* bunch, FReplicationFlags* rep_flags) override;
	virtual void TickComponent(float delta, ELevelTick tick_type,
							   FActorComponentTickFunction* tick_function) override;
	// INTERFACE_END(UActorComponent) 

public:
	UFUNCTION(BlueprintCallable, Category = "Action")
	void AddAction(TSubclassOf<UARAction> action_class, AActor* instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	void RemoveAction(const FName& name);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool HasAction(const FName& name) const;

	// |all_instances| determines whether we're starting all the instances of just the first one.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	bool StartAction(const FName& name, AActor* instigator, bool all_instances = false);

	// |all_instances| determines whether we're starting all the instances of just the first one.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	bool StopAction(const FName& name, AActor* instigator, bool all_instances = false);


protected:
	// INTERFACE_BEGIN(UARActionComponent) 
	virtual bool StartAction_Implementation(const FName& name, AActor* instigator,
											bool all_instances);
	virtual bool StopAction_Implementation(const FName& name, AActor* instigator,
										   bool all_instances);

	UFUNCTION()
	void OnRep_Actions(TArray<UARAction*> old_actions);
	// INTERFACE_END(UARActionComponent)

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UPROPERTY(ReplicatedUsing="OnRep_Actions", EditAnywhere)
	TArray<TObjectPtr<UARAction>> Actions;

	// SERVER_PROPERTIES_BEGIN ---------------------------------------------------------------------
	// Default actions will be created upon startup.
	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSoftClassPtr<UARAction>> Server_DefaultActions;
	// SERVER_PROPERTIES_END -----------------------------------------------------------------------
};