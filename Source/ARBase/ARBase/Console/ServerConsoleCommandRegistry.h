#pragma once

#include <ARBase/ClientServerSplit.h>

#include <memory>

namespace ar
{

class ConsoleCommandRegistry;
using ConsoleCommandCallback = std::function<void(APlayerController*, const TArray<FString>&)>;

#if AR_BUILD_CLIENT
namespace client
{

class ConsoleCommandRegistryClient
{
	GENERATED_LEAF_CLIENT_SPLIT(ConsoleCommandRegistry, ConsoleCommandRegistryClient);
};

} // namespace client
#endif // AR_BUILD_CLIENT

#if AR_BUILD_SERVER
namespace server
{

struct ServerConsoleCommand
{
	FName Name;
	FString Help;
	ConsoleCommandCallback Callback;
};

class ConsoleCommandRegistryServer
{
	GENERATED_LEAF_SERVER_SPLIT(ConsoleCommandRegistry, ConsoleCommandRegistryServer);

public:
	void RegisterConsoleCommand(const TCHAR* name, const TCHAR* help,
								ConsoleCommandCallback&& callback);

private:
	TMap<FName, std::unique_ptr<ServerConsoleCommand>> CommandMap;
};

} // namespace server
#endif // AR_BUILD_SERVER

class ARBASE_API ConsoleCommandRegistry
{
	GENERATED_BASE_CLIENT_SPLIT(ConsoleCommandRegistry, ar::client::ConsoleCommandRegistryClient);
	GENERATED_BASE_SERVER_SPLIT(ConsoleCommandRegistry, ar::server::ConsoleCommandRegistryServer);

public:
	static ConsoleCommandRegistry& Get();

public:
	ConsoleCommandRegistry();

public:
	void RegisterServerConsoleCommand(const TCHAR* name, const TCHAR* help,
									  ConsoleCommandCallback&& callback);
};

} // namespace ar