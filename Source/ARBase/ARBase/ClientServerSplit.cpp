#include <ARBase/ClientServerSplit.h>

#if AR_BUILD_DEDICATED_CLIENT

bool ARClientServerGlobals::RunningAsClient(NotNullPtr<AActor>)
{
	static_assert(false, "Support this case! Currently we only support listen server for now");
	return true;
}

bool ARClientServerGlobals::RunningAsServer(NotNullPtr<AActor>)
{
	static_assert(false, "Support this case! Currently we only support listen server for now");
	return false;
}

#elsif AR_BUILD_DEDICATED_SERVER

bool ARClientServerGlobals::RunningAsClient(NotNullPtr<AActor>)
{
	static_assert(false, "Support this case! Currently we only support listen server for now");
	return false;
}

bool ARClientServerGlobals::RunningAsServer(NotNullPtr<AActor>)
{
	static_assert(false, "Support this case! Currently we only support listen server for now");
	return true;
}

#else // !AR_BUILD_DEDICATED_CLIENT && !AR_BUILD_DEDICATED_SERVER

// For now we assume that if we don't have authority, we are running as client.
// TODO(cdc): When we begin with client side things, this will not be true. For now, this is true.

bool ARClientServerGlobals::RunningInClient(NotNullPtr<AActor> actor)
{
	return !actor->HasAuthority();
}

bool ARClientServerGlobals::RunningInServer(NotNullPtr<AActor> actor)
{
	return actor->HasAuthority();
}

#endif