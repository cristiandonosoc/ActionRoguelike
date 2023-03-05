// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ARGame/Widgets/ARWidgetSubsystem.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "ARGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARGAME_API UARGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	virtual void OnStart() override;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> WidgetConfigData;
};
