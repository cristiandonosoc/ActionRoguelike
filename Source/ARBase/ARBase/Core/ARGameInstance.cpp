// Fill out your copyright notice in the Description page of Project Settings.

#include <ARBase/Core/ARGameInstance.h>

#include <ARBase/NotNullPtr.h>
#include <ARBase/Subsystems/ARWidgetSubsystem.h>

namespace
{

template <typename T>
TMap<FName, T*> GetAllRowsWithName(NotNullPtr<UDataTable> dt)
{
	if (!dt->GetRowStruct() || !dt->GetRowStruct()->IsChildOf(T::StaticStruct()))
	{
		return {};
	}

	TMap<FName, T> rows;
	rows.Reserve(dt->GetRowMap().Num());

	for (const auto& [key, ptr] : dt->GetRowMap())
	{
		rows.Add(key, reinterpret_cast<T*>(ptr));
	}

	return rows;
}

} // namespace

NotNullPtr<UARGameInstance> UARGameInstance::GetInstance(NotNullPtr<const UObject> outer)
{
	NotNullPtr<UWorld> world = outer->GetWorld();
	return world->GetGameInstance<UARGameInstance>();
}

void UARGameInstance::OnStart()
{
	Super::OnStart();

	if (ensure(WidgetConfigData))
	{
		TArray<FWidgetSubsystemConfig*> rows;
		WidgetConfigData->GetAllRows<FWidgetSubsystemConfig>(TEXT("UARGAMEInstance Start"), rows);

		NotNullPtr widget_manager = GetSubsystem<UARWidgetSubsystem>();
		widget_manager->LoadWidgetClasses(std::move(rows));
	}

	MessagingManager.Start(this);
}
void UARGameInstance::Shutdown()
{
	Super::Shutdown();

	MessagingManager.Shutdown();
}
