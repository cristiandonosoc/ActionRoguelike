#pragma once

#include <ARBase/ClientServerSplit.h>

class AARBaseProjectile;

namespace ar
{
namespace client
{

class ARGAMECLIENT_API BaseProjectileClient
{
	GENERATED_LEAF_CLIENT_SPLIT(AARBaseProjectile);

public:
	void BeginPlay();
};


} // namespace client
} // namespace ar


