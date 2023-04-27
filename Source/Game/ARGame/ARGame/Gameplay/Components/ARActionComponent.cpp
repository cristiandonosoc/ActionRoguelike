#include <ARGame/Gameplay/Components/ARActionComponent.h>

#include <ARBase/Logging.h>
#include <ARBase/NotNullPtr.h>
#include <ARBase/Subsystems/ARStreamingSubsystem.h>
#include <ARGame/ARDebugCategories.h>
#include <ARGame/Gameplay/Actions/ARAction.h>
#include <Net/UnrealNetwork.h>

// Sets default values for this component's properties
UARActionComponent::UARActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UARActionComponent::BeginPlay()
{
	Super::BeginPlay();
	INIT_BASE_CLIENT_SERVER_SPLIT();

	SERVER_ONLY_CALL(BeginPlay);
}

void UARActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& props) const
{
	Super::GetLifetimeReplicatedProps(props);

	// Unreal expects this stupid ass name.
	auto& OutLifetimeProps = props;
	DOREPLIFETIME(UARActionComponent, Actions);
	DOREPLIFETIME(UARActionComponent, ActiveGameplayTags);
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

		{
			FString msg =
				FString::Printf(TEXT("%s: %s"), *GetNameSafe(GetOwner()),
								*FString::JoinBy(Actions, TEXT(","),
												 [](const auto& action) -> FString
												 { return *action->GetActionName().ToString(); }));
			debug::DrawText(ar::ACTIONS, GetWorld(), std::move(msg), FColor::White);
		}

		{
			FString msg = FString::Printf(TEXT("%s: %s"), *GetNameSafe(GetOwner()),
										  *ActiveGameplayTags.ToStringSimple());
			debug::DrawText(ar::ACTIONS, GetWorld(), std::move(msg), FColor::Blue);
		}
	}
}

void UARActionComponent::ServerAddAction(TSubclassOf<UARAction> action_class, AActor* instigator)
{
	CHECK_RUNNING_ON_SERVER(this);
	check(action_class);

	SERVER_ONLY_CALL(AddAction, action_class, instigator);
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

void UARActionComponent::ServerStartAction(const FName& name, AActor* instigator,
										   FPredictedStartActionContext context)
{
	CHECK_RUNNING_ON_SERVER(this);
	SERVER_ONLY_CALL(StartActionByName, name, instigator, std::move(context));
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
															   AActor* instigator,
															   FPredictedStartActionContext context)
{
	CHECK_RUNNING_ON_SERVER(this);
	check(action);

	SERVER_ONLY_CALL(StartAction, action, instigator, std::move(context));
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
	// TODO(cdc): Likely start should be handled in the action directly rather than through the
	//            component, so that we're correctly synced with the replication.
	if (!action)
	{
		AR_LOG_CSS(GetWorld(), LogAR_Actions, Error,
				   TEXT("Starting action that has not been replicated yet. Most likely because of "
						"auto start."));
		return;
	}
	CLIENT_ONLY_CALL(StartAction, action, instigator);
}

void UARActionComponent::RPC_Multicast_StopAction_Implementation(UARAction* action,
																 AActor* instigator)
{
	check(action);
	CLIENT_ONLY_CALL(StopAction, action, instigator);
}