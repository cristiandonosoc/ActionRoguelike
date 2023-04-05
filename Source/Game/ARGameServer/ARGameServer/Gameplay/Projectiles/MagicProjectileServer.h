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