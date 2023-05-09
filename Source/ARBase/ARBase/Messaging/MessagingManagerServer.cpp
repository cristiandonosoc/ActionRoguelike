#include <ARBase/Messaging/MessagingManagerServer.h>

#include <ARBase/Core/ARPlayerController.h>
#include <ARBase/Messaging/MessagingManager.h>

#ifdef AR_BUILD_SERVER

namespace ar
{
namespace server
{


void MessagingManagerServer::OnNewConnection(NotNullPtr<AARPlayerController> player_controller,
											 NotNullPtr<UNetConnection> connection)
{
	// We should not be tracking this connection already.
	auto it = std::find_if(GetBase()->Connections.begin(), GetBase()->Connections.end(),
						   [connection](const auto& tracker) -> bool
						   { return tracker.Connection == connection.Get(); });
	check(it == GetBase()->Connections.end());

	// We add it to the tracking.
	MessagingManager::ConnectionTracker tracker = {};
	tracker.PlayerController = player_controller;
	tracker.Connection = connection;
	GetBase()->Connections.push_back(std::move(tracker));


	// For each channel we're tracking, we create an UChannelWrapper.
	for (const auto& [channel_id, channel] : GetBase()->MessageChannels)
	{
		checkf(false, TEXT("TODO(cdc): Implement"));
	}
}

UARGameInstance* MessagingManagerServer::GetGameInstance()
{
	check(GetBase()->GameInstance);
	return GetBase()->GameInstance;
}

} // namespace server
} // namespace ar

#endif // AR_BUILD_SERVER
