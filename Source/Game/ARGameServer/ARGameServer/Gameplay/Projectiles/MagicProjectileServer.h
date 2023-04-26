#pragma once

#include <ARBase/ClientServerSplit.h>

class AARMagicProjectile;

namespace ar
{
namespace server
{

class ARGAMESERVER_API MagicProjectileServer
{
	GENERATED_LEAF_SERVER_SPLIT(AARMagicProjectile, MagicProjectileServer);

public:
	// TODO(cdc): We should not go about this in this manner.
	//            We should let the client know some effect needs to run before the other side gets
	//            finally destroyed.
	static constexpr float kDestroyDelay = 0.25f;

public:
	void OnBeginHit(const FHitResult& hit, AActor* hit_actor);

private:
	void MarkForDestruction();

private:
	FTimerHandle DestroyTimerHandle;
};

} // namespace server
} // namespace ar