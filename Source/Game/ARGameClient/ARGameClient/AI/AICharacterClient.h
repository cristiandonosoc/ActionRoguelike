#pragma once

#include <ARBase/ClientServerSplit.h>

class AARAICharacter;
class UAREnemyHealthBarWidget;

struct FOnHealthChangedPayload;

namespace ar
{
namespace client
{

class ARGAMECLIENT_API AICharacterClient
{
	GENERATED_LEAF_CLIENT_SPLIT(AARAICharacter, AICharacterClient);

public:
	void OnHealthChanged(const FOnHealthChangedPayload& payload);

private:
	TObjectPtr<UAREnemyHealthBarWidget> HealthBarWidget;
};

} // namespace client
} // namespace ar