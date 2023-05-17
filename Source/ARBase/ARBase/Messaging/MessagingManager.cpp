#include <ARBase/Messaging/MessagingManager.h>

#include <ARBase/Core/ARPlayerController.h>
#include <ARBase/Messaging/MessageEndpoint.h>

namespace ar
{

MessagingManager::MessagingManager()
{
	INIT_BASE_CLIENT_SERVER_SPLIT();
}

void MessagingManager::Start(UARGameInstance* game_instance)
{
	GameInstance = game_instance;

	// We create all the endpoints that have been registered.
	for (const auto& [endpoint_id, entry] : GetGlobalMessageEndpointRegistry())
	{
		CreateMessageEndpoint(endpoint_id);
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

void MessagingManager::CreateMessageEndpoint(const FName& endpoint_id)
{
	check(!MessageEndpoints.Contains(endpoint_id));
	auto channel = std::make_unique<MessageEndpoint>();
	MessageEndpoints[endpoint_id] = std::move(channel);
}

void MessagingManager::DestroyMessageEndpoint(const FName& endpoint_id)
{
	check(MessageEndpoints.Contains(endpoint_id));
	MessageEndpoints.Remove(endpoint_id);
}

} // namespace ar
