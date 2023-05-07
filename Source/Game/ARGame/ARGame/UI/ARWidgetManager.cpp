#include <ARGame/UI/ARWidgetManager.h>

#include <ARBase/NotNullPtr.h>
#include <ARBase/Subsystems/ARWidgetSubsystem.h>
#include <ARGame/UI/Widgets/ARDamagePopupWidget.h>
#include <ARGame/UI/Widgets/AREnemyHealthBarWidget.h>

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

/* static */ UARDamagePopupWidget* UARWidgetManager::CreateDamagePopupWidget(AActor* attached_to,
																			 float damage)
{
	check(attached_to);
	auto* damage_popup = CreateBaseWidget<UARDamagePopupWidget>(attached_to);
	if (!damage_popup)
	{
		return nullptr;
	}
	damage_popup->Damage = FMath::Abs(damage);
	return damage_popup;
}

/* static */ UAREnemyHealthBarWidget*
UARWidgetManager::CreateEnemyHealthBarWidget(AActor* attached_to, float current_health,
											 float max_health)
{
	check(attached_to);
	auto* widget = CreateBaseWidget<UAREnemyHealthBarWidget>(attached_to);
	if (!widget)
	{
		return nullptr;
	}
	widget->CurrentHealth = current_health;
	widget->MaxHealth = max_health;
	return widget;
}
