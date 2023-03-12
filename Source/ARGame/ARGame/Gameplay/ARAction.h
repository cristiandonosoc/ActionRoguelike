#pragma once

#include <CoreMinimal.h>
#include <UObject/Object.h>

#include "ARAction.generated.h"

UCLASS()
class ARGAME_API UARAction : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void Start(AActor* instigator);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void Stop(AActor* instigator);

	const FName& GetActionName() const { return ActionName; }

protected:
	// INTERFACE_BEGIN(UARAction)
	virtual void Start_Implementation(AActor* instigator);
	virtual void Stop_Implementation(AActor* instigator);
	// INTERFACE_END(UARAction)

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;
};