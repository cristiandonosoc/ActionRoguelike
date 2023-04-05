#pragma once

#include <ARBase/ClientServerSplit.h>

class UARAttributeComponent;

namespace ar
{
namespace client
{

class ARGAMECLIENT_API AttributeComponentClient
{
	GENERATED_LEAF_CLIENT_SPLIT(UARAttributeComponent, AttributeComponentClient);
};

} // namespace client
} // namespace ar