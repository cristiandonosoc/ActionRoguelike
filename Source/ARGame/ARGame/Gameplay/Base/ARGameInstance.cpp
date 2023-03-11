// Fill out your copyright notice in the Description page of Project Settings.

#include <ARGame/Gameplay/Base/ARGameInstance.h>

#include <ARBase/NotNullPtr.h>
#include <ARGame/Subsystems/ARWidgetSubsystem.h>

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
}