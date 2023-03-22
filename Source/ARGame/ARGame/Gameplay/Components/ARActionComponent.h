﻿#pragma once

#include <Components/ActorComponent.h>
#include <CoreMinimal.h>
#include <GameplayTagContainer.h>
#include <Kismet/BlueprintPlatformLibrary.h>

#include "ARActionComponent.generated.h"

class UARAction;

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class ARGAME_API UARActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UARActionComponent();

	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void AddAction(TSubclassOf<UARAction> action_class);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void RemoveAction(const FName& name);

	// |all_instances| determines whether we're starting all the instances of just the first one.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	bool StartAction(const FName& name, AActor* instigator, bool all_instances = false);

	// |all_instances| determines whether we're starting all the instances of just the first one.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	bool StopAction(const FName& name, AActor* instigator, bool all_instances = false);


	FGameplayTagContainer& GetActiveGameplayTags() { return ActiveGameplayTags; }

protected:
	// INTERFACE_BEGIN(UARActionComponent)
	virtual void AddAction_Implementation(TSubclassOf<UARAction> action_class);
	virtual void RemoveAction_Implementation(const FName& name);
	virtual bool StartAction_Implementation(const FName& name, AActor* instigator,
											bool all_instances);
	virtual bool StopAction_Implementation(const FName& name, AActor* instigator,
										   bool all_instances);
	// INTERFACE_END(UARActionComponent)

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	// Default actions will be created upon startup.
	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSoftClassPtr<UARAction>> DefaultActions;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UARAction>> Actions;
};