#pragma once

#include <CoreMinimal.h>
#include <UObject/Object.h>

#include "ARWidgetManager.generated.h"

class UAREnemyHealthBarWidget;
class UARDamagePopupWidget;

UCLASS()
class ARGAME_API UARWidgetManager : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static UARDamagePopupWidget* CreateDamagePopupWidget(AActor* attached_to, float damage);

	UFUNCTION(BlueprintCallable)
	static UAREnemyHealthBarWidget*
	CreateEnemyHealthBarWidget(AActor* attached_to, float current_health, float max_health);
};
