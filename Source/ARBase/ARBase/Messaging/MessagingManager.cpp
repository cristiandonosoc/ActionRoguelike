#include <ARBase/Messaging/MessagingManager.h>

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

void MessagingManager::CreateChannel(const FName& channel_id)
{
	check(!ChannelMap.Contains(channel_id));
	auto channel = std::make_unique<MessageChannel>();
	ChannelMap[channel_id] = std::move(channel);
}

void MessagingManager::DestroyChannel(const FName& channel_id)
{
	check(ChannelMap.Contains(channel_id));
	ChannelMap.Remove(channel_id);
}

} // namespace ar
