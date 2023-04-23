#include <ARGame/Gameplay/Base/ARPlayerState.h>

#include <ARBase/ClientServerSplit.h>

#include <Net/UnrealNetwork.h>

AARPlayerState::AARPlayerState()
{
	bReplicates = true;
}

void AARPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& props) const
{
	Super::GetLifetimeReplicatedProps(props);

	// Unreal expects this stupid ass name.
	auto& OutLifetimeProps = props;
	DOREPLIFETIME(AARPlayerState, Credits);
}

int32 AARPlayerState::ChangeCredits(int32 change)
{
	Credits += change;
	OnPlayerCreditsChanged.Broadcast(Credits);
	return Credits;
}

void AARPlayerState::OnRep_Credits(int32 old_credits)
{
	CHECK_RUNNING_ON_CLIENT(this);
	OnPlayerCreditsChanged.Broadcast(Credits);
}