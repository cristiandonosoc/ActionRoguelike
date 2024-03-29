#pragma once

#include <BehaviorTree/BTTaskNode.h>
#include <CoreMinimal.h>

#include "ARBTTask_Hide.generated.h"

class AARAICharacter;

struct FEnvQueryResult;
class UEnvQuery;

// TODO(cdc): This is currently broken because of the way it is performing the MoveTo.
//            Recommended to not use.
UCLASS()
class ARGAME_API UARBTTask_Hide : public UBTTaskNode
{
	GENERATED_BODY()
public:
	struct Context
	{
		int32 QueryID = INDEX_NONE;
		TSharedPtr<FEnvQueryResult> Result;
	};

	UARBTTask_Hide(const FObjectInitializer&);

protected:
	virtual uint16 GetInstanceMemorySize() const override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner,
											uint8* node_memory) override;
	virtual void TickTask(UBehaviorTreeComponent& owner, uint8* node_memory, float delta) override;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UEnvQuery> FindHidingSpotEQS;

private:
	void ReceiveResult(TSharedPtr<FEnvQueryResult>);
};