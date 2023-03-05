// Fill out your copyright notice in the Description page of Project Settings.


#include "ARWidgetSubsystem.h"


#include "ARBase/NotNullPtr.h"
#include "ARBase/Sybsystems/ARStreamingSubsystem.h"
#include "ARDamagePopupWidget.h"
#include "Blueprint/UserWidget.h"

void UARWidgetSubsystem::CreateDamagePopup(AActor* attached_to, float damage)
{
	check(attached_to);

	auto widget_class = DamageWidgetClass.Get();
	if (!widget_class)
	{
		UE_LOG(LogTemp, Error, TEXT("Damage Widget class not set"));
		return;
	}

	NotNullPtr damage_widget = CreateWidget<UARDamagePopupWidget>(GetWorld(), widget_class);
	damage_widget->CPP_AttachedTo = attached_to;
	damage_widget->CPP_Damage = damage;

	damage_widget->AddToViewport();
}

void UARWidgetSubsystem::InitializeWidgets(const FWidgetSubsystemConfig& config)
{
	// TODO(cdc): Do async loading.
	NotNullPtr streamer = GetGameInstance()->GetSubsystem<UARStreamingSubsystem>();
	if (!config.DamageWidgetClass.IsNull())
	{
		DamageWidgetClass = streamer->RequestSyncLoad(config.DamageWidgetClass);
		check(DamageWidgetClass);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Damage widget class is null!"));
	}
}