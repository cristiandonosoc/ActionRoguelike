#include <ARBase/Messaging/MessagingManagerClient.h>

#ifdef AR_BUILD_CLIENT

namespace ar
{
namespace client
{
void MessagingManagerClient::OnNewConnection(NotNullPtr<AARPlayerController> player_controller,
											 NotNullPtr<UNetConnection> connection)
{
	checkf(false, TEXT("TODO(cdc): Implement"));
}
} // namespace client
} // namespace ar

#endif // AR_BUILD_CLIENT
