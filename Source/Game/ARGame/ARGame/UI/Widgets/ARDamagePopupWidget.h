#pragma once

#include <ARBase/UI/ARActorAttachedWidget.h>
#include <ARBase/Subsystems/ARWidgetSubsystem.h>

#include <CoreMinimal.h>

#include "ARDamagePopupWidget.generated.h"

UCLASS()
class ARGAME_API UARDamagePopupWidget : public UARActorAttachedWidget
{
	GENERATED_BODY()

public:
	static EARWidgetType GetStaticType() { return EARWidgetType::DamagePopup; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;
};
