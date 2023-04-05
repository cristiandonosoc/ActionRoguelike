#pragma once

#include <ARBase/ClientServerSplit.h>

class AARCharacter;
class UARAction_ProjectileAttack;

namespace ar
{
namespace client
{

class ARGAMECLIENT_API Action_ProjectileAttackClient
{
	GENERATED_LEAF_CLIENT_SPLIT(UARAction_ProjectileAttack);

public:
	void Start(NotNullPtr<AActor> instigator);

private:
	void AttackTimerEnd(NotNullPtr<AARCharacter> instigator);

private:
	// This is the timer associated with the wait needed to spawn the projectile.
	// TODO(cdc): Use animation notifications.
	FTimerHandle TimerHandle;
};

} // namespace client
} // namespace ar
