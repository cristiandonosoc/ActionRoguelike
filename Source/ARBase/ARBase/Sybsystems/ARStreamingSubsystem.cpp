// Fill out your copyright notice in the Description page of Project Settings.


#include "ARBase/Sybsystems/ARStreamingSubsystem.h"

void UARStreamingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}


void UARStreamingSubsystem::RequestSyncLoad(const TSoftClassPtr<UObject>& class_ptr)
{
	if (class_ptr.IsValid())
	{
		return;
	}
	
	StreamManager.LoadSynchronous(class_ptr);
}