#include <ARBase/Messaging/MessagingManagerServer.h>

#include <ARBase/Messaging/MessagingManager.h>

#ifdef AR_BUILD_SERVER

namespace ar
{
namespace server
{

void MessagingManagerServer::Start()
{
	GetGameInstance()->GetOnNet

}

UARGameInstance* MessagingManagerServer::GetGameInstance()
{
	check(GetBase()->GameInstance);
	return GetBase()->GameInstance;
}

} // namespace server
} // namespace ar

#endif // AR_BUILD_SERVER
