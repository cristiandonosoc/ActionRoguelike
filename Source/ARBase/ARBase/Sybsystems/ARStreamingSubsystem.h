// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "ARStreamingSubsystem.generated.h"

// UARStreamingSubsystem is a singleton system that takes care of streaming of assets and presents
// a common API for systems to use.
UCLASS()
class ARBASE_API UARStreamingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void RequestSyncLoad(const TSoftClassPtr<UObject>& class_ptr);

private:
	FStreamableManager StreamManager;
};
