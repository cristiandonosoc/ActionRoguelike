#pragma once

#include <ARBase/Subsystems/ARWidgetSubsystem.h>
#include <ARBase/UI/ARActorAttachedWidget.h>

#include <CoreMinimal.h>

#include "AREnemyHealthBarWidget.generated.h"

UCLASS()
class ARGAME_API UAREnemyHealthBarWidget : public UARActorAttachedWidget
{
	GENERATED_BODY()

public:
	static EARWidgetType GetStaticType() { return EARWidgetType::EnemyHealthBar; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;
};
