#pragma once

#include <ARBase/DebugTools.h>

class UWorld;

namespace ar
{
namespace logging
{

// GetClientServerPrefix gets the prefix depending on the world.
// It supports passing a null world. In that case, a "no world" prefix will be generated.
ARBASE_API const FString& GetClientServerPrefix(UWorld* world);

ARBASE_API void Log(UWorld* world, FLogCategoryBase& category, ELogVerbosity::Type verbosity,
					const char* file, int line, FString&& msg);

} // namespace logging
} // namespace ar

#define AR_LOG(category, verbosity, format, ...)                                                   \
	AR_LOG_CSS(nullptr, category, verbosity, format, ##__VA_ARGS__);

#define AR_LOG_CSS(world, category, verbosity, format, ...)                                        \
	do                                                                                             \
	{                                                                                              \
		FString msg = FString::Printf(format, ##__VA_ARGS__);                                      \
		::ar::logging::Log(world, category, ELogVerbosity::verbosity, __FILE__, __LINE__,          \
						   std::move(msg));                                                        \
	}                                                                                              \
	while (false)
