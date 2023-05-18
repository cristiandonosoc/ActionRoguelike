#include <ARBase/Messaging/MessagingManagerClient.h>

#ifdef AR_BUILD_CLIENT

namespace ar
{
namespace client
{

void MessagingManagerClient::OnChannelCreated(const FMessageChannelCreatedEventData& data)
{
	checkf(false, TEXT("TODO(cdc): Implement"));
}

} // namespace client
} // namespace ar

#endif // AR_BUILD_CLIENT
