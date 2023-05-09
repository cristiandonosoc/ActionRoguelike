#include <ARBase/Core/ARPlayerController.h>

#include <ARBase/Core/ARGameInstance.h>

void AARPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if (NetConnection)
	{
		auto& messaging_manager = UARGameInstance::GetInstance(this)->GetMessagingManager();
		messaging_manager.OnNewConnection(this, NetConnection.Get());
	}
}
