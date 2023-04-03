#pragma once

#include <ARBase/ClientServerSplit.h>

class AARMagicProjectile;

namespace ar
{
namespace server
{

class ARGAMESERVER_API MagicProjectileServer
{
	GENERATED_LEAF_SERVER_SPLIT(AARMagicProjectile);

public:
	void OnBeginHit(const FHitResult& hit, AActor* hit_actor);
};

} // namespace server
} // namespace ar