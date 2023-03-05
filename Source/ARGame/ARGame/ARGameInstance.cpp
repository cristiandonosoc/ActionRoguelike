// Fill out your copyright notice in the Description page of Project Settings.


#include "ARGame/ARGameInstance.h"

#include "ARBase/NotNullPtr.h"

void UARGameInstance::OnStart()
{
	Super::OnStart();

	if (ensure(WidgetConfigData))
	{
		NotNullPtr widget_manager = GetSubsystem<UARWidgetSubsystem>();
		TArray<FWidgetSubsystemConfig*> rows;
		WidgetConfigData->GetAllRows(TEXT("Widget manager initialization"), rows);
		check(rows.Num() == 1);
		widget_manager->InitializeWidgets(*rows[0]);
	}
}