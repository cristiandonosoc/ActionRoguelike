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
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void Start(AActor* instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void Stop(AActor* instigator);

	const FName& GetActionName() const { return ActionName; }
	bool GetIsRunning() const { return IsRunning; }

	UARActionComponent& GetOwningComponent();
	virtual UWorld* GetWorld() const override;

protected:
	// INTERFACE_BEGIN(UARAction)
	virtual bool CanStart_Implementation(AActor* instigator);
	virtual void Start_Implementation(AActor* instigator);
	virtual void Stop_Implementation(AActor* instigator);
	// INTERFACE_END(UARAction)

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool CanBeStartedMultipleTimes = false;

	UPROPERTY(BlueprintReadOnly, Category = "Action")
	bool IsRunning = false;

	// Tags added to the owning actor when |Start| is called. Removed on |Stop|.
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	// Action can only start if owning actor has none of this tags applied.
	// TODO(cdc): This seems like it really is a decision for the caller to have, not something an
	//            action should decide.
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;
};