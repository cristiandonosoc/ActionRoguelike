#include <ARGame/Gameplay/Components/ARActionComponent.h>

#include <ARBase/NotNullPtr.h>
#include <ARBase/Subsystems/ARStreamingSubsystem.h>
#include <ARGame/ARDebugCategories.h>
#include <ARGame/Gameplay/Actions/ARAction.h>
#include <Net/UnrealNetwork.h>

// Sets default values for this component's properties
UARActionComponent::UARActionComponent()
{
	INIT_BASE_CLIENT_SERVER_SPLIT();

	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

const TArray<TSoftClassPtr<UARAction>>& UARActionComponent::GetServer_DefaultActions() const
{
	CHECK_RUNNING_ON_SERVER(this);
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
	CHECK_RUNNING_ON_SERVER(this);
	return GetServerSplit()->ReplicateSubObjects(channel, bunch, rep_flags);
}

void UARActionComponent::TickComponent(float delta, ELevelTick tick_type,
									   FActorComponentTickFunction* tick_function)
{
	Super::TickComponent(delta, tick_type, tick_function);

	if (debug::IsCategoryEnabled(ar::ACTIONS))
	{
		// FString msg = FString::Printf(TEXT("%s: %s"), *GetNameSafe(GetOwner()),
		// 							  *ActiveGameplayTags.ToStringSimple());
		// debug::DrawText(ar::ACTIONS, GetWorld(), std::move(msg), FColor::Blue);

		FString msg =
			FString::Printf(TEXT("%s: %s"), *GetNameSafe(GetOwner()),
							*FString::JoinBy(Actions, TEXT(","),
											 [](const auto& action) -> FString
											 { return *action->GetActionName().ToString(); }));
		debug::DrawText(ar::ACTIONS, GetWorld(), std::move(msg), FColor::White);
	}
}

void UARActionComponent::AddAction(TSubclassOf<UARAction> action_class, AActor* instigator)
{
	check(action_class);


	NotNullPtr action = NewObject<UARAction>(this, action_class.Get());
	Actions.Add(action);

	UE_LOG(LogAR_Actions, Log, TEXT("Adding action %s"), *action->GetActionName().ToString());

	if (action->GetAutoStarts())
	{
		ClientPredictStartAction(action->GetActionName(), instigator);
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

UARAction* UARActionComponent::FindAction(const FName& name) const
{
	auto result = Actions.FindByPredicate([&name](const TObjectPtr<UARAction>& action)
										  { return action->GetActionName() == name; });
	return result ? result->Get() : nullptr;
}

void UARActionComponent::ClientPredictStartAction(const FName& name, AActor* instigator)
{
	CHECK_RUNNING_ON_CLIENT(this);
	CLIENT_ONLY_CALL(PredictStartActionByName, name, instigator);
}

void UARActionComponent::ServerStartAction(const FName& name, AActor* instigator)
{
	CHECK_RUNNING_ON_SERVER(this);
	SERVER_ONLY_CALL(StartActionByName, name, instigator);
}

void UARActionComponent::StopAction(const FName& name, AActor* instigator)
{
	NotNullPtr<UARAction> action = FindAction(name);
	CLIENT_SERVER_CALL(StopAction, action, instigator);
}

void UARActionComponent::OnRep_Actions(TArray<UARAction*> old_actions)
{
	CHECK_RUNNING_ON_CLIENT(this);
}

void UARActionComponent::RPC_Server_StartAction_Implementation(UARAction* action,
															   AActor* instigator)
{
	CHECK_RUNNING_ON_SERVER(this);
	check(action);

	SERVER_ONLY_CALL(StartAction, action, instigator);
}

void UARActionComponent::RPC_Server_StopAction_Implementation(UARAction* action, AActor* instigator)
{
	CHECK_RUNNING_ON_SERVER(this);
	check(action);

	SERVER_ONLY_CALL(StopAction, action, instigator);
}

void UARActionComponent::RPC_Multicast_StartAction_Implementation(UARAction* action,
																  AActor* instigator)
{
	check(action);

	// This is a client only RPC.
	if (ARClientServerGlobals::RunningInServer(this))
	{
		return;
	}

	CLIENT_ONLY_CALL(StartAction, action, instigator);
}

void UARActionComponent::RPC_Multicast_StopAction_Implementation(UARAction* action,
																 AActor* instigator)
{
	check(action);

	// This is a client only RPC.
	if (ARClientServerGlobals::RunningInServer(this))
	{
		return;
	}

	CLIENT_ONLY_CALL(StopAction, action, instigator);
}