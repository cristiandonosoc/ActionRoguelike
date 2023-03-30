// Fill out your copyright notice in the Description page of Project Settings.

#include <ARGame/AI/BehaviorTree/ARBTTask_Hide.h>

#include <ARBase/NotNullPtr.h>

#include <AIController.h>
#include <EnvironmentQuery/EnvQueryManager.h>

#include <map>

namespace ARBTTask_Hide_Private
{

static constexpr float TickTime = 0.1f;

struct QueryManager
{
	std::map<int32, UARBTTask_Hide::Context*> Queries;
};

static NotNullPtr<QueryManager> GetQueryManager()
{
	static QueryManager manager = {};
	return &manager;
}

} // namespace ARBTTask_Hide_Private

UARBTTask_Hide::UARBTTask_Hide(const FObjectInitializer& oi) : Super(oi)
{
	NodeName = "Hide";
	bTickIntervals = true;
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

uint16 UARBTTask_Hide::GetInstanceMemorySize() const
{
	return sizeof(Context);
}

EBTNodeResult::Type UARBTTask_Hide::ExecuteTask(UBehaviorTreeComponent& owner, uint8* node_memory)
{
	if (!ensure(FindHidingSpotEQS))
	{
		return EBTNodeResult::Failed;
	}

	NotNullPtr<AAIController> ai_controller = owner.GetAIOwner();
	NotNullPtr<APawn> ai_pawn = ai_controller->GetPawn();

	// Create the hiding request.
	FEnvQueryRequest request(FindHidingSpotEQS, ai_pawn);
	int32 query_id =
		request.Execute(EEnvQueryRunMode::SingleResult, this, &UARBTTask_Hide::ReceiveResult);

	// Track our memory in the request tracker.
	auto manager = ARBTTask_Hide_Private::GetQueryManager();
	Context* context = reinterpret_cast<Context*>(node_memory);
	*context = {};
	context->QueryID = query_id;
	manager->Queries[query_id] = context;

	SetNextTickTime(node_memory, ARBTTask_Hide_Private::TickTime);

	UE_LOG(LogTemp, Log, TEXT("[Query %d] Scheduled Hide task with query"), query_id);
	return EBTNodeResult::InProgress;
}

void UARBTTask_Hide::TickTask(UBehaviorTreeComponent& owner, uint8* node_memory, float delta)
{
	Super::TickTask(owner, node_memory, delta);

	NotNullPtr<Context> context = reinterpret_cast<Context*>(node_memory);
	if (!context->Result)
	{
		UE_LOG(LogTemp, Log, TEXT("[Query %d] Tick: Result is not there yet"),
			   context->Result->QueryID);
		SetNextTickTime(node_memory, ARBTTask_Hide_Private::TickTime);
		return;
	}

	if (!context->Result->IsFinished())
	{
		UE_LOG(LogTemp, Log, TEXT("[Query %d] Tick: Query is not finished yet"),
			   context->Result->QueryID);
		SetNextTickTime(node_memory, ARBTTask_Hide_Private::TickTime);
		return;
	}

	if (!context->Result->IsSuccessful())
	{
		UE_LOG(LogTemp, Log, TEXT("[Query %d] Tick: Query failed"),
			   context->Result->QueryID);
		FinishLatentTask(owner, EBTNodeResult::Failed);
		return;
	}

	NotNullPtr<AAIController> ai = owner.GetAIOwner();
	FVector target_location = context->Result->GetItemAsLocation(0);
	check(!target_location.IsNearlyZero());
	switch (ai->MoveToLocation(target_location))
	{
	case EPathFollowingRequestResult::Failed:
		UE_LOG(LogTemp, Log, TEXT("[Query %d] Tick: (location: %s), Pathfinding failed"),
			   context->Result->QueryID, *target_location.ToString());
		FinishLatentTask(owner, EBTNodeResult::Failed);
		return;
	case EPathFollowingRequestResult::AlreadyAtGoal:
		UE_LOG(LogTemp, Log, TEXT("[Query %d] Tick: (location: %s) Pathfinding succeeded"),
			   context->Result->QueryID, *target_location.ToString());
		FinishLatentTask(owner, EBTNodeResult::Succeeded);
		return;
	case EPathFollowingRequestResult::RequestSuccessful:
		UE_LOG(LogTemp, Log, TEXT("[Query %d] Tick: (location: %s) Request successful"),
			   context->Result->QueryID, *target_location.ToString());
		SetNextTickTime(node_memory, ARBTTask_Hide_Private::TickTime);
		return;
	}
}

void UARBTTask_Hide::ReceiveResult(TSharedPtr<FEnvQueryResult> result)
{
	check(result);
	UE_LOG(LogTemp, Log, TEXT("Received query result for query %d"), result->QueryID);
	auto manager = ARBTTask_Hide_Private::GetQueryManager();
	Context* context = manager->Queries[result->QueryID];
	check(context);
	context->Result = std::move(result);
}