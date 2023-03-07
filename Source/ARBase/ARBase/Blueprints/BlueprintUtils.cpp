#include <ARBase/Blueprints/BlueprintUtils.h>

float UBlueprintUtils::GetAccurateTime()
{
	return FApp::GetCurrentTime() - GStartTime;
}
