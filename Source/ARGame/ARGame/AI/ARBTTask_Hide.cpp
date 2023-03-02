// Fill out your copyright notice in the Description page of Project Settings.


#include "ARGame/AI/ARBTTask_Hide.h"

#include <map>
	
#include "AIController.h"
#include "ARBase/NotNullPtr.h"
#include "EnvironmentQuery/EnvQueryManager.h"


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
	QueryId = request.Execute(EEnvQueryRunMode::SingleResult, this, &UARBTTask_Hide::ReceiveResult);

	// Track our memory in the request tracker.
	auto manager = ARBTTask_Hide_Private::GetQueryManager();
	Context* context = reinterpret_cast<Context*>(node_memory);
	*context = {};
	manager->Queries[QueryId] = context;

	SetNextTickTime(node_memory, ARBTTask_Hide_Private::TickTime);
	return EBTNodeResult::InProgress;
}

void UARBTTask_Hide::TickTask(UBehaviorTreeComponent& owner, uint8* node_memory, float delta)
{
	Super::TickTask(owner, node_memory, delta);

	Context* context = reinterpret_cast<Context*>(node_memory);
	if (!context || !context->Result)
	{
		SetNextTickTime(node_memory, ARBTTask_Hide_Private::TickTime);
		return;
	}

	if (!context->Result->IsSuccessful())
	{
		FinishLatentTask(owner, EBTNodeResult::Failed);
		return;
	}

	NotNullPtr<AAIController> ai = owner.GetAIOwner();
	switch (ai->MoveToLocation(context->Result->GetItemAsLocation(0)))
	{
	case EPathFollowingRequestResult::Failed:
		FinishLatentTask(owner, EBTNodeResult::Failed);
		return;
	case EPathFollowingRequestResult::AlreadyAtGoal:
		FinishLatentTask(owner, EBTNodeResult::Succeeded);
		return;
	case EPathFollowingRequestResult::RequestSuccessful:
		SetNextTickTime(node_memory, ARBTTask_Hide_Private::TickTime);
		return;
	}
}

void UARBTTask_Hide::ReceiveResult(TSharedPtr<FEnvQueryResult> result)
{
	check(result);
	auto manager = ARBTTask_Hide_Private::GetQueryManager();
	Context* context = manager->Queries[result->QueryID];
	check(context);
	context->Result = std::move(result);
}