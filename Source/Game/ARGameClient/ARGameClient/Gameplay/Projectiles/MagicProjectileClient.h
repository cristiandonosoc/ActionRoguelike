#pragma once

#include <ARBase/ClientServerSplit.h>

class AARMagicProjectile;

namespace ar
{
namespace client
{

class ARGAMECLIENT_API MagicProjectileClient
{
	GENERATED_LEAF_CLIENT_SPLIT(AARMagicProjectile, MagicProjectileClient);

	void OnBeginHit(const FHitResult& hit, AActor* other_actor);
};

} // namespace client
} // namespace ar
