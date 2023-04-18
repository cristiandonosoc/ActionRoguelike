#pragma once

#include <ARBase/ClientServerSplit.h>


struct FPredictedStartActionContext;
class AARCharacter;
class UARAction_ProjectileAttack;

namespace ar
{
namespace client
{

class ARGAMECLIENT_API Action_ProjectileAttackClient
{
	GENERATED_LEAF_CLIENT_SPLIT(UARAction_ProjectileAttack, Action_ProjectileAttackClient);

public:
	void PredictStart(NotNullPtr<AActor> instigator,
					  NotNullPtr<FPredictedStartActionContext> out_context);
	// void Start(NotNullPtr<AActor> instigator);
};

} // namespace client
} // namespace ar