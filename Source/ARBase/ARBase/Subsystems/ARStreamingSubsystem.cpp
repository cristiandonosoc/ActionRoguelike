#include <ARBase/Subsystems/ARStreamingSubsystem.h>

void UARStreamingSubsystem::RequestSyncLoad(const TArray<FSoftObjectPath>& paths)
{
	StreamManager.RequestSyncLoad(paths, false);
}