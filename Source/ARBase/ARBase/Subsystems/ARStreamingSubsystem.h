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
	static TArray<FSoftObjectPath>
	ExtractSoftObjectPaths(const TArray<TSoftClassPtr<T>>& soft_classes);

	template <typename T>
	TSubclassOf<T> RequestSyncLoad(const TSoftClassPtr<T>& class_ptr);

	void RequestSyncLoad(const TArray<FSoftObjectPath>& paths);

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

template <typename T>
TArray<FSoftObjectPath>
UARStreamingSubsystem::ExtractSoftObjectPaths(const TArray<TSoftClassPtr<T>>& soft_classes)
{
	TArray<FSoftObjectPath> paths;
	paths.Reserve(soft_classes.Num());

	for (const TSoftClassPtr<T>& soft_class : soft_classes)
	{
		paths.Add(soft_class.ToSoftObjectPath());
	}

	return paths;
}