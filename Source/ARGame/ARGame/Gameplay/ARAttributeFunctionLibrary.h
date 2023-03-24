#pragma once

#include <CoreMinimal.h>

#include "ARAttributeFunctionLibrary.generated.h"

UCLASS()
class ARGAME_API UARAttributeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	// Attributes.
public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static bool ApplyDamage(AActor* instigator, AActor* target, float damage);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static bool ApplyDirectionalDamage(AActor* instigator, AActor* target, float damage,
									   const FHitResult& hit);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static bool ApplyHeal(AActor* instigator, AActor* target, float heal);
};