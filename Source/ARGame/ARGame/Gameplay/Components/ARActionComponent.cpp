#include <ARGame/Gameplay/Components/ARActionComponent.h>

#include <ARBase/NotNullPtr.h>
#include <ARBase/Subsystems/ARStreamingSubsystem.h>
#include <ARGame/ARDebugCategories.h>
#include <ARGame/Gameplay/Actions/ARAction.h>

AR_REGISTER_DEBUG_CATEGORY(ARDebugCategories::ACTIONS, true, "All about actions");

// Sets default values for this component's properties
UARActionComponent::UARActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UARActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// TODO(cdc): Do async loading.
	NotNullPtr streamer = GetWorld()->GetGameInstance()->GetSubsystem<UARStreamingSubsystem>();
	auto paths = UARStreamingSubsystem::ExtractSoftObjectPaths(DefaultActions);
	streamer->RequestSyncLoad(paths);

	// Now that the classes are loaded, we can create the objects.
	for (const TSoftClassPtr<UARAction> action_soft_class : DefaultActions)
	{
		UClass* uclass = action_soft_class.Get();
		check(uclass);

		AddAction(TSubclassOf<UARAction>(uclass));
	}
}

void UARActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
									   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ARDebugDraw::IsCategoryEnabled(ARDebugCategories::ACTIONS))
	{
		FString msg = FString::Printf(TEXT("%s: %s"), *GetNameSafe(GetOwner()),
									  *ActiveGameplayTags.ToStringSimple());
		ARDebugDraw::Text(ARDebugCategories::ACTIONS, std::move(msg), FColor::Blue);
	}
}

void UARActionComponent::AddAction_Implementation(TSubclassOf<UARAction> action_class)
{
	check(action_class);

	NotNullPtr action = NewObject<UARAction>(this, action_class.Get());
	Actions.Add(std::move(action));
}

bool UARActionComponent::StartAction_Implementation(const FName& name, AActor* instigator,
													bool all_instances)
{
	// We search for one action that matches the parameters.
	bool found = false;
	for (const TObjectPtr<UARAction> action : Actions)
	{
		if (action->GetActionName() != name)
		{
			continue;
		}
		
		found = true;
		if (!action->CanStart(instigator))
		{
			FString msg = FString::Printf(TEXT("Action %s cannot start"), *name.ToString());
			ARDebugDraw::Text(ARDebugCategories::ACTIONS, msg, FColor::Red, 2);
			continue;
		}

		action->Start(instigator);
		if (!all_instances)
		{
			return true;
		}
	}

	if (!found)
	{
		FString msg = FString::Printf(TEXT("No action for name %s found"), *name.ToString());
		ARDebugDraw::Text(ARDebugCategories::ACTIONS, msg, FColor::Red, 2);
	}

	return found;
}

bool UARActionComponent::StopAction_Implementation(const FName& name, AActor* instigator,
												   bool all_instances)
{
	bool found = false;
	for (const TObjectPtr<UARAction> action : Actions)
	{
		if (action->GetActionName() != name)
		{
			continue;
		}

		// We only want to stop actions that are running.
		if (!action->GetIsRunning())
		{
			continue;
		}

		found = true;
		action->Stop(instigator);
		if (!all_instances)
		{
			return true;
		}
	}

	return found;
}