#include <ARBase/Console/ServerConsoleCommandRegistry.h>

namespace ar
{


ConsoleCommandRegistry& ConsoleCommandRegistry::Get()
{
	static ConsoleCommandRegistry registry;
	return registry;
}

ConsoleCommandRegistry::ConsoleCommandRegistry()
{
	INIT_BASE_CLIENT_SERVER_SPLIT();
}

void ConsoleCommandRegistry::RegisterServerConsoleCommand(const TCHAR* name, const TCHAR* help,
														  ConsoleCommandCallback&& callback)
{
#if AR_BUILD_SERVER
	GetServerSplit()->RegisterConsoleCommand(name, help, std::move(callback));
#endif // AR_BUILD_SERVER
}

#if AR_BUILD_CLIENT
namespace client
{


} // namespace client
#endif // AR_BUILD_CLIENT

#if AR_BUILD_SERVER
namespace server
{

void ConsoleCommandRegistryServer::RegisterConsoleCommand(const TCHAR* name, const TCHAR* help,
														  ConsoleCommandCallback&& callback)
{
	FName fname(name);
	check(!CommandMap.Contains(fname));

	// We add the command to our tracking.
	auto command = std::make_unique<ServerConsoleCommand>();
	command->Name = fname;
	command->Help = help;
	command->Callback = std::move(callback);
	CommandMap[fname] = std::move(command);
}

} // namespace server
#endif // AR_BUILD_SERVER

} // namespace ar