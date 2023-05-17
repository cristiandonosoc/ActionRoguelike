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
	check(std::find_if(GetBase()->ConnectionTracker.begin(), GetBase()->ConnectionTracker.end(),
					   [connection](const auto& entry) -> bool {
						   return entry->NetConnection.Get() == connection.Get();
					   }) == GetBase()->ConnectionTracker.end());

	// We add it to the tracking.
	auto entry = std::make_unique<MessagingManager::ConnectionTrackerEntry>();
	entry->PlayerController = player_controller;
	entry->NetConnection = connection;

	// TODO(cdc): For each channel we're tracking, we create an UMessageChannel.

	GetBase()->ConnectionTracker.push_back(std::move(entry));
}

UARGameInstance* MessagingManagerServer::GetGameInstance()
{
	check(GetBase()->GameInstance);
	return GetBase()->GameInstance;
}

} // namespace server
} // namespace ar

#endif // AR_BUILD_SERVER