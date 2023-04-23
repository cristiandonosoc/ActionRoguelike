#include <ARBase/Logging.h>

#include <map>

namespace ar
{
namespace logging
{

const FString& GetClientServerPrefix(UWorld* world)
{
	// We see if we already have calculated this.
	if (!world)
	{
		static FString noWorldPrefix(TEXT("<No World>"));
		return noWorldPrefix;
	}

	static std::map<UWorld*, FString> worldPrefixMap;
	if (auto it = worldPrefixMap.find(world); it != worldPrefixMap.end())
	{
		return it->second;
	}

	// Otherwise we need to calculate the prefix.
	FString prefix;
	switch (world->GetNetMode())
	{
	case NM_Client:
		// GPlayInEditorID 0 is always the server, so 1 will be first client.
		// You want to keep this logic in sync with GeneratePIEViewportWindowTitle and
		// UpdatePlayInEditorWorldDebugString
		prefix = FString::Printf(TEXT("Client %d"), GPlayInEditorID);
		break;
	case NM_DedicatedServer:
	case NM_ListenServer:
		prefix = FString::Printf(TEXT("Server"));
		break;
	default:
		check(false);
		break;
	}

	// We save it for future uses.
	worldPrefixMap[world] = std::move(prefix);
	auto it = worldPrefixMap.find(world);
	check(it != worldPrefixMap.end());
	return it->second;
}

void Log(UWorld* world, FLogCategoryBase& category, ELogVerbosity::Type verbosity, const char* file,
		 int line, FString&& msg)
{
	const FString& prefix = GetClientServerPrefix(world);

	// Sadly we need to do this stupid thing because the log macro expects to do a direct text
	// append to verbosity...
	switch (verbosity)
	{
	case ELogVerbosity::Fatal:
		UE_LOG_REF(category, Fatal, TEXT("%s: %s [%s:%d]"), *prefix, *msg, *FString(file), line);
		break;
	case ELogVerbosity::Error:
		UE_LOG_REF(category, Error, TEXT("%s: %s [%s:%d]"), *prefix, *msg, *FString(file), line);
		break;
	case ELogVerbosity::Warning:
		UE_LOG_REF(category, Warning, TEXT("%s: %s [%s:%d]"), *prefix, *msg, *FString(file), line);
		break;
	case ELogVerbosity::Display:
		UE_LOG_REF(category, Display, TEXT("%s: %s [%s:%d]"), *prefix, *msg, *FString(file), line);
		break;
	case ELogVerbosity::Log:
		UE_LOG_REF(category, Log, TEXT("%s: %s [%s:%d]"), *prefix, *msg, *FString(file), line);
		break;
	case ELogVerbosity::Verbose:
		UE_LOG_REF(category, Verbose, TEXT("%s: %s [%s:%d]"), *prefix, *msg, *FString(file), line);
		break;
	case ELogVerbosity::VeryVerbose:
		UE_LOG_REF(category, VeryVerbose, TEXT("%s: %s [%s:%d]"), *prefix, *msg, *FString(file),
				   line);
		break;
	default:
		break;
	}
}

} // namespace logging
} // namespace ar