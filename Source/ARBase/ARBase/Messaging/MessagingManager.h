#pragma once

#include <ARBase/ClientServerSplit.h>
#include <ARBase/Messaging/MessageChannel.h>

#ifdef AR_BUILD_CLIENT
#include <ARBase/Messaging/MessagingManagerClient.h>
#endif // AR_BUILD_CLIENT
#ifdef AR_BUILD_SERVER
#include <ARBase/Messaging/MessagingManagerServer.h>
#endif // AR_BUILD_SERVER

class UARGameInstance;

namespace ar
{

class MessagingManager
{
	GENERATED_BASE_CLIENT_SPLIT(MessagingManager, ar::client::MessagingManagerClient);
	GENERATED_BASE_SERVER_SPLIT(MessagingManager, ar::server::MessagingManagerServer);

public:
	using MessageChannelMap = TMap<FName, std::unique_ptr<MessageChannel>>;

public:
	MessagingManager();

public:
	const MessageChannelMap& GetMessageChannelMap() const { return ChannelMap; }

public:
	void Start(UARGameInstance* game_instance);
	void Shutdown();

	void CreateChannel(const FName& channel_id);
	void DestroyChannel(const FName& channel_id);

private:
	UARGameInstance* GameInstance = nullptr; // Not owning.
	TMap<FName, std::unique_ptr<MessageChannel>> ChannelMap;
};

} // namespace ar
