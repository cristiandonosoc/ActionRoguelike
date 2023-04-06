#include <ARGame/Gameplay/Components/ARActionComponent.h>

#include <ARBase/NotNullPtr.h>
#include <ARBase/Subsystems/ARStreamingSubsystem.h>
#include <ARGame/ARDebugCategories.h>
#include <ARGame/Gameplay/Actions/ARAction.h>
#include <Net/UnrealNetwork.h>

AR_DECLARE_DEBUG_CATEGORY(ACTIONS, ARDebugCategories::ACTIONS, true, "All about actions");

// Sets default values for this component's properties
UARActionComponent::UARActionComponent()
{
	INIT_BASE_CLIENT_SERVER_SPLIT();
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

const TArray<TSoftClassPtr<UARAction>>& UARActionComponent::GetServer_DefaultActions() const
{
	CHECK_RUNNING_ON_SERVER();
	return Server_DefaultActions;
}

void UARActionComponent::BeginPlay()
{
	Super::BeginPlay();

	SERVER_ONLY_CALL(BeginPlay);
}

void UARActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& props) const
{
	Super::GetLifetimeReplicatedProps(props);

	// Unreal expects this stupid ass name.
	auto& OutLifetimeProps = props;
	DOREPLIFETIME(UARActionComponent, Actions);
}

bool UARActionComponent::ReplicateSubobjects(UActorChannel* channel, FOutBunch* bunch,
											 FReplicationFlags* rep_flags)
{
	CHECK_RUNNING_ON_SERVER();
	return GetServerSplit()->ReplicateSubObjects(channel, bunch, rep_flags);
}

void UARActionComponent::TickComponent(float delta, ELevelTick tick_type,
									   FActorComponentTickFunction* tick_function)
{
	Super::TickComponent(delta, tick_type, tick_function);

	if (ARDebugDraw::IsCategoryEnabled(ARDebugCategories::ACTIONS))
	{
		// FString msg = FString::Printf(TEXT("%s: %s"), *GetNameSafe(GetOwner()),
		// 							  *ActiveGameplayTags.ToStringSimple());
		// ARDebugDraw::Text(ARDebugCategories::ACTIONS, GetWorld(), std::move(msg), FColor::Blue);

		FString msg =
			FString::Printf(TEXT("%s: %s"), *GetNameSafe(GetOwner()),
							*FString::JoinBy(Actions, TEXT(","),
											 [](const auto& action) -> FString
											 { return *action->GetActionName().ToString(); }));
		ARDebugDraw::Text(ARDebugCategories::ACTIONS, GetWorld(), std::move(msg), FColor::White);
	}
}

void UARActionComponent::AddAction(TSubclassOf<UARAction> action_class, AActor* instigator)
{
	check(action_class);


	NotNullPtr action = NewObject<UARAction>(this, action_class.Get());
	Actions.Add(action);

	UE_LOG(LogTemp, Log, TEXT("Adding action %s"), *action->GetActionName().ToString());

	if (action->GetAutoStarts())
	{
		StartAction(action->GetActionName(), instigator, false);
	}
}

void UARActionComponent::RemoveAction(const FName& name)
{
	int32 index = INDEX_NONE;
	for (int32 i = 0; i < Actions.Num(); i++)
	{
		if (Actions[i]->GetActionName() == name)
		{
			index = i;
			break;
		}
	}

	check(index != INDEX_NONE);
	check(Actions[index]->GetIsRunning());

	Actions.RemoveAt(index);
}
bool UARActionComponent::HasAction(const FName& name) const
{
	return Actions.ContainsByPredicate([&name](const TObjectPtr<UARAction>& action)
									   { return action->GetActionName() == name; });
}

bool UARActionComponent::StartAction_Implementation(const FName& name, AActor* instigator,
													bool all_instances)
{
	// We search for one action that matches the parameters.
	bool found = false;
	for (const TObjectPtr<UARAction>& action : Actions)
	{
		if (action->GetActionName() != name)
		{
			continue;
		}

		found = true;
		if (!action->CanStart(instigator))
		{
			FString msg = FString::Printf(TEXT("Action %s cannot start"), *name.ToString());
			ARDebugDraw::Text(ARDebugCategories::ACTIONS, GetWorld(), msg, FColor::Red, 2);
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
		ARDebugDraw::Text(ARDebugCategories::ACTIONS, GetWorld(), msg, FColor::Red, 2);
	}

	return found;
}

bool UARActionComponent::StopAction_Implementation(const FName& name, AActor* instigator,
												   bool all_instances)
{
	bool found = false;
	for (const TObjectPtr<UARAction>& action : Actions)
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

void UARActionComponent::OnRep_Actions(TArray<UARAction*> old_actions)
{
	CHECK_RUNNING_ON_CLIENT();
}