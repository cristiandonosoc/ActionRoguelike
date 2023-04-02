#pragma once

#include <ARBase/ClientServerSplit.h>

class AARCharacter;

namespace ar
{
namespace server
{

class ARGAMESERVER_API CharacterServer
{
	GENERATED_SERVER_SPLIT(AARCharacter);
};

} // namespace server
} // namespace ar