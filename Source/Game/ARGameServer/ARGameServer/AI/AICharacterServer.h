#pragma once

#include <ARBase/ClientServerSplit.h>

struct FOnHealthChangedPayload;
class AARAICharacter;

namespace ar
{
namespace server
{

class ARGAMESERVER_API AICharacterServer
{
	GENERATED_LEAF_SERVER_SPLIT(AARAICharacter, AICharacterServer);

private:
	// Time in seconds until the dead actor is finally removed.
	static constexpr float kDeathDelay = 10.0f;

public:
	bool PerformPrimaryAttack(const AActor& target);
	void OnHealthChanged(const FOnHealthChangedPayload& payload);
};


} // namespace server
} // namespace ar