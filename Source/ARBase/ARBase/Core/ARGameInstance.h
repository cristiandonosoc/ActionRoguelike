#pragma once

#include <ARBase/Messaging/MessagingManager.h>

#include <CoreMinimal.h>
#include <Engine/GameInstance.h>

#include "ARGameInstance.generated.h"

class UDataTable;

UCLASS()
class ARBASE_API UARGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	const ar::MessagingManager& GetMessagingManager() const { return MessagingManager; }

protected:
	virtual void OnStart() override;
	virtual void Shutdown() override;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> WidgetConfigData;

private:
	ar::MessagingManager MessagingManager;
};
