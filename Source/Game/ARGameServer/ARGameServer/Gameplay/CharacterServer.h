﻿#pragma once

#include <ARBase/ClientServerSplit.h>

class AARCharacter;

namespace ar
{
namespace server
{

class ARGAMESERVER_API CharacterServer
{
	GENERATED_LEAF_SERVER_SPLIT(AARCharacter, CharacterServer);
};

} // namespace server
} // namespace ar