#pragma once

#include <ARBase/ClientServerSplit.h>
#include <ARGame/Gameplay/Components/ARActionComponentData.h>
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
	void ServerAddAction(TSubclassOf<UARAction> action_class, AActor* instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	void RemoveAction(const FName& name);

	UFUNCTION(BlueprintCallable, Category = "Action")
	UARAction* FindAction(const FName& name) const;

	UFUNCTION(BlueprintCallable, Category = "Action")
	void ClientPredictStartAction(const FName& name, AActor* instigator);

	UFUNCTION(BlueprintCallable, Category = "Action|Server")
	void ServerStartAction(const FName& name, AActor* instigator,
						   FPredictedStartActionContext context);

	UFUNCTION(BlueprintCallable, Category = "Action")
	void StopAction(const FName& name, AActor* instigator);

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UPROPERTY(Replicated, EditAnywhere)
	TArray<TObjectPtr<UARAction>> Actions;

	// SERVER_PROPERTIES_BEGIN ---------------------------------------------------------------------
	// Default actions will be created upon startup.
	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSoftClassPtr<UARAction>> Server_DefaultActions;
	// SERVER_PROPERTIES_END -----------------------------------------------------------------------
};