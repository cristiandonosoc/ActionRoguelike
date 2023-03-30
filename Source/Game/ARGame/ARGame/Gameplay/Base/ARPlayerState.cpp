#include <ARGame/Gameplay/Base/ARPlayerState.h>

int32 AARPlayerState::ChangeCredits(int32 change)
{
	Credits += change;
	OnPlayerCreditsChanged.Broadcast(Credits);
	return Credits;
}