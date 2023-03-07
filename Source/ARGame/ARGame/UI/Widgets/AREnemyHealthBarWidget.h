// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <ARGame/UI/ARActorAttachedWidget.h>
#include <ARGame/Subsystems/ARWidgetSubsystem.h>

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