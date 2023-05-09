#include <ARBase/Messaging/MessagingManager.h>

#include <ARBase/Core/ARPlayerController.h>
#include <ARBase/Messaging/MessageChannel.h>

namespace ar
{

MessagingManager::MessagingManager()
{
	INIT_BASE_CLIENT_SERVER_SPLIT();
}

void MessagingManager::Start(UARGameInstance* game_instance)
{
	GameInstance = game_instance;

	// We create all the channels that have been registered.
	for (const auto& [channel_id, entry] : GetGlobalMessageChannelRegistry())
	{
		CreateChannel(channel_id);
	}
}

void MessagingManager::Shutdown()
{
	check(GameInstance);
	GameInstance = nullptr;
}

void MessagingManager::OnNewConnection(NotNullPtr<AARPlayerController> player_controller,
									   NotNullPtr<UNetConnection> connection)
{
	check(GameInstance);
	if (ARClientServerGlobals::RunningInClient(player_controller))
	{
		GetClientSplit()->OnNewConnection(player_controller, connection);
	}
	if (ARClientServerGlobals::RunningInServer(player_controller))
	{
		GetServerSplit()->OnNewConnection(player_controller, connection);
	}
}

void MessagingManager::CreateChannel(const FName& channel_id)
{
	check(!MessageChannels.Contains(channel_id));
	auto channel = std::make_unique<MessageChannel>();
	MessageChannels[channel_id] = std::move(channel);
}

void MessagingManager::DestroyChannel(const FName& channel_id)
{
	check(MessageChannels.Contains(channel_id));
	MessageChannels.Remove(channel_id);
}

} // namespace ar
