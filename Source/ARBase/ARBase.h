#pragma once

#include <CoreMinimal.h>

class FBaseModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
