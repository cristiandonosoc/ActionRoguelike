﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ARDamagePopupWidget.h"
#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "ARWidgetManager.generated.h"

UCLASS()
class ARGAME_API UARWidgetManager : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static UARDamagePopupWidget* CreateDamagePopupWidget(AActor* attached_to, float damage);
};
