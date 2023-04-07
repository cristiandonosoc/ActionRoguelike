#pragma once

#include <ARBase/ClientServerSplit.h>

class AARCharacter;
class UARAction_ProjectileAttack;

namespace ar
{
namespace server
{

class ARGAMESERVER_API Action_ProjectileAttackServer
{
	GENERATED_LEAF_SERVER_SPLIT(UARAction_ProjectileAttack);

public:
	void Start(NotNullPtr<AARCharacter> instigator, const FVector& location,
			   const FRotator& rotation);
};

} // namespace server
} // namespace ar
