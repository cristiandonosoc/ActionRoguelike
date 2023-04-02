#pragma once

#include <ARBase/ClientServerSplit.h>

class UARAttributeComponent;

namespace ar
{
namespace server
{

class ARGAMESERVER_API AttributeComponentServer
{
	GENERATED_LEAF_SERVER_SPLIT(UARAttributeComponent);

public:
	bool ApplyHealthChange(NotNullPtr<AActor> instigator, float delta);
};

} // namespace server
} // namespace ar