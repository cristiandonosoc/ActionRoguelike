// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "ARGameInstance.generated.h"

class UDataTable;

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
