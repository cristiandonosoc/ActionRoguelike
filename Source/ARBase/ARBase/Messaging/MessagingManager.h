#pragma once

#include <ARBase/ClientServerSplit.h>
#include <ARBase/Messaging/MessageChannel.h>

#ifdef AR_BUILD_CLIENT
#include <ARBase/Messaging/MessagingManagerClient.h>
#endif // AR_BUILD_CLIENT
#ifdef AR_BUILD_SERVER
#include <ARBase/Messaging/MessagingManagerServer.h>
#endif // AR_BUILD_SERVER

class AARPlayerController;
class UARGameInstance;

namespace ar
{

class MessagingManager
{
	GENERATED_BASE_CLIENT_SPLIT(MessagingManager, ar::client::MessagingManagerClient);
	GENERATED_BASE_SERVER_SPLIT(MessagingManager, ar::server::MessagingManagerServer);

public:
	using MessageChannelMap = TMap<FName, std::unique_ptr<MessageChannel>>;
	using UChannelWrapperMap = TMap<FName, std::vector<TWeakObjectPtr<UChannelWrapper>>>;


public:
	MessagingManager();

public:
	void Start(UARGameInstance* game_instance);
	void Shutdown();

	void OnNewConnection(NotNullPtr<AARPlayerController> player_controller,
						 NotNullPtr<UNetConnection> connection);

	void CreateChannel(const FName& channel_id);
	void DestroyChannel(const FName& channel_id);

private:
	UARGameInstance* GameInstance = nullptr; // Not owning.

	MessageChannelMap MessageChannels;
	UChannelWrapperMap UChannelWrappers;

	// The actual player connections we're tracking.
	// On client: Would be one connection, to the server.
	// On server: Would have many connections, one per connected client.
	struct ConnectionTracker
	{
		TWeakObjectPtr<AARPlayerController> PlayerController;
		TWeakObjectPtr<UNetConnection> Connection;
	};
	std::vector<ConnectionTracker> Connections;
};

} // namespace ar
