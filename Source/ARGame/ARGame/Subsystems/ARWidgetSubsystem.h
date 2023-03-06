// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "ARWidgetSubsystem.generated.h"


class UARActorAttachedWidget;
class UUserWidget;

UENUM(BlueprintType)
enum class EARWidgetType : uint8
{
	NONE, 
	DamagePopup,
	
	COUNT UMETA(Hidden),
};

USTRUCT(BlueprintType)
struct FWidgetSubsystemConfig : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EARWidgetType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UARActorAttachedWidget> ActorAttachedWidgetClass;
};

UCLASS()
class ARGAME_API UARWidgetSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	UARActorAttachedWidget* CreateActorAttachedWidget(const EARWidgetType& type,
													  AActor* attached_to);

	void LoadWidgetClasses(const TArray<FWidgetSubsystemConfig*>& rows);

private:
	// Cache of all the actor attached classes.
	TMap<EARWidgetType, TSubclassOf<UARActorAttachedWidget>> ActorAttachedWidgetClasses;
};