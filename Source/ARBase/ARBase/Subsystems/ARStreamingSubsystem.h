// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Engine/StreamableManager.h>
#include <Subsystems/GameInstanceSubsystem.h>

#include "ARStreamingSubsystem.generated.h"

// UARStreamingSubsystem is a singleton system that takes care of streaming of assets and presents
// a common API for systems to use.
UCLASS()
class ARBASE_API UARStreamingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	template <typename T>
	TSubclassOf<T> RequestSyncLoad(const TSoftClassPtr<T>& class_ptr);

private:
	FStreamableManager StreamManager;
};


template <typename T>
TSubclassOf<T> UARStreamingSubsystem::RequestSyncLoad(const TSoftClassPtr<T>& class_ptr)
{
	if (class_ptr.IsValid())
	{
		return class_ptr.Get();
	}
	
	return StreamManager.LoadSynchronous(class_ptr);
}
