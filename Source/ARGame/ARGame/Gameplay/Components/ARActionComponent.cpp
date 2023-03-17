#include <ARGame/Gameplay/Components/ARActionComponent.h>

#include <ARBase/NotNullPtr.h>
#include <ARBase/Subsystems/ARStreamingSubsystem.h>
#include <ARGame/Gameplay/Actions/ARAction.h>

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

void UARActionComponent::AddAction_Implementation(TSubclassOf<UARAction> action_class)
{
	check(action_class);

	NotNullPtr action = NewObject<UARAction>(this, action_class.Get());
	Actions.Add(std::move(action));
}

bool UARActionComponent::StartAction_Implementation(const FName& name, AActor* instigator,
													bool all_instances)
{
	bool found = false;
	for (const TObjectPtr<UARAction> action : Actions)
	{
		if (action->GetActionName() == name)
		{
			found = true;
			action->Start(instigator);
			if (!all_instances)
			{
				return true;
			}
		}
	}

	return found;
}

bool UARActionComponent::StopAction_Implementation(const FName& name, AActor* instigator,
												   bool all_instances)
{
	bool found = false;
	for (const TObjectPtr<UARAction> action : Actions)
	{
		if (action->GetActionName() == name)
		{
			found = true;
			action->Stop(instigator);
			if (!all_instances)
			{
				return true;
			}
		}
	}

	return found;
}