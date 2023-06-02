#pragma once

#include <ARBase/ClientServerSplit.h>
#include <ARBase/Messaging/MessageEndpoint.h>

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
	MessagingManager();

public:
	void Init(UARGameInstance* game_instance);
	void Shutdown();

public:
	void CreateMessageEndpoint(const FName& endpoint_id);
	void DestroyMessageEndpoint(const FName& endpoint_id);

	void OnNewConnection(NotNullPtr<AARPlayerController> player_controller,
						 NotNullPtr<UNetConnection> connection);

private:
	UARGameInstance* GameInstance = nullptr; // Not owning.

	TMap<FName, std::unique_ptr<MessageEndpoint>> MessageEndpoints;

	// The actual player connections we're tracking.
	// On client: Would be one connection, to the server.
	// On server: Would have many connections, one per connected client.
	struct ConnectionTrackerEntry
	{
		TWeakObjectPtr<AARPlayerController> PlayerController;
		TWeakObjectPtr<UNetConnection> NetConnection;
		TMap<FName, std::vector<TWeakObjectPtr<UMessageChannel>>> MessageChannels;
	};
	std::vector<std::unique_ptr<ConnectionTrackerEntry>> ConnectionTracker;
};

} // namespace ar