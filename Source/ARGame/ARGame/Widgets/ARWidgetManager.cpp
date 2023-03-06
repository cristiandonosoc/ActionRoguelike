// Fill out your copyright notice in the Description page of Project Settings.


#include "ARWidgetManager.h"


#include "ARBase/NotNullPtr.h"
#include "ARDamagePopupWidget.h"
#include "ARGame/Subsystems/ARWidgetSubsystem.h"

namespace
{

template <typename T>
T* CreateBaseWidget(NotNullPtr<AActor> attached_to)
{
	NotNullPtr widget_subsystem =
		attached_to->GetWorld()->GetGameInstance()->GetSubsystem<UARWidgetSubsystem>();
	return Cast<T>(widget_subsystem->CreateActorAttachedWidget(T::GetStaticType(), attached_to));
}

} // namespace

/*static*/ UARDamagePopupWidget* UARWidgetManager::CreateDamagePopupWidget(AActor* attached_to,
																		   float damage)
{
	check(attached_to);
	auto* damage_popup = CreateBaseWidget<UARDamagePopupWidget>(attached_to);
	check(damage_popup);
	damage_popup->Damage = FMath::Abs(damage);
	return damage_popup;
}