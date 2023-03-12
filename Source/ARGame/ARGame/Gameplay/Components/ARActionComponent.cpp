#include <ARGame/Gameplay/Components/ARActionComponent.h>

#include <ARBase/NotNullPtr.h>
#include <ARGame/Gameplay/ARAction.h>

// Sets default values for this component's properties
UARActionComponent::UARActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
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