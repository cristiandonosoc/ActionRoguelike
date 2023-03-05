// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "ARWidgetSubsystem.generated.h"

class UUserWidget;
class UARDamagePopupWidget;

USTRUCT(BlueprintType)
struct FWidgetSubsystemConfig : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UARDamagePopupWidget> DamageWidgetClass;
};

UCLASS()
class ARGAME_API UARWidgetSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CreateDamagePopup(AActor* attached_to, float damage);

	void InitializeWidgets(const FWidgetSubsystemConfig&);

private:
	TSubclassOf<UARDamagePopupWidget> DamageWidgetClass;
};
