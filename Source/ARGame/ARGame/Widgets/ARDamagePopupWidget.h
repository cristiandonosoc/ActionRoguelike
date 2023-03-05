// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"

#include "ARDamagePopupWidget.generated.h"

UCLASS()
class ARGAME_API UARDamagePopupWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* CPP_AttachedTo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CPP_Damage;
};
