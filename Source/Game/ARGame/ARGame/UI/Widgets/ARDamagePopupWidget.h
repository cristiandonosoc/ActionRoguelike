// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <ARGame/UI/ARActorAttachedWidget.h>
#include <ARGame/Subsystems/ARWidgetSubsystem.h>

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