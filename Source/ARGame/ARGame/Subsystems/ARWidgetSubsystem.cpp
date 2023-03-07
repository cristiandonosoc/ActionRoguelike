// Fill out your copyright notice in the Description page of Project Settings.

#include <ARGame/Subsystems/ARWidgetSubsystem.h>

#include <ARBase/NotNullPtr.h>
#include <ARBase/Subsystems/ARStreamingSubsystem.h>
#include <ARGame/UI/ARActorAttachedWidget.h>
#include <Blueprint/UserWidget.h>

void UARWidgetSubsystem::LoadWidgetClasses(const TArray<FWidgetSubsystemConfig*>& rows)
{
	NotNullPtr streamer = GetGameInstance()->GetSubsystem<UARStreamingSubsystem>();

	ActorAttachedWidgetClasses.Reserve(rows.Num());
	for (const auto& row : rows)
	{
		if (row->ActorAttachedWidgetClass.IsNull())
		{
			UE_LOG(LogTemp, Warning, TEXT("Class ptr for widget \"%s\" is null"),
				   *UEnum::GetValueAsString(row->Type));
			continue;
		}

		const auto& soft_class_ptr = row->ActorAttachedWidgetClass;

		// TODO(cdc): Do async loading.
		auto subclass_ptr = streamer->RequestSyncLoad(soft_class_ptr);
		if (!subclass_ptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not stream class %s (key: %s)"),
				   *soft_class_ptr.ToString(), *UEnum::GetValueAsString(row->Type));
			continue;
		}

		// Finally we add the pointer.
		check(!ActorAttachedWidgetClasses.Contains(row->Type));

		UE_LOG(LogTemp, Log, TEXT("Loaded widget class %s (key: %s)"), *subclass_ptr->GetName(),
			   *UEnum::GetValueAsString(row->Type));
		ActorAttachedWidgetClasses.Add(row->Type, std::move(subclass_ptr));
	}
}


UARActorAttachedWidget* UARWidgetSubsystem::CreateActorAttachedWidget(const EARWidgetType& type,
																	  AActor* attached_to)
{
	check(attached_to);

	auto* widget_class = ActorAttachedWidgetClasses.Find(type);
	if (!widget_class)
	{
		UE_LOG(LogTemp, Error, TEXT("Widget class by key %s not loaded"),
			   *UEnum::GetValueAsString(type));
		return nullptr;
	}

	auto widget = CreateWidget<UARActorAttachedWidget>(GetWorld(), *widget_class);
	if (!widget)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not create widget with class %s (key: %s)"),
			   *GetNameSafe(*widget_class), *UEnum::GetValueAsString(type));
		return nullptr;
	}

	widget->AttachedTo = attached_to;
	return widget;
}