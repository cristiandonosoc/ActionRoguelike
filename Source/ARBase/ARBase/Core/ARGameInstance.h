#pragma once

#include <ARBase/Messaging/MessagingManager.h>

#include <CoreMinimal.h>
#include <Engine/GameInstance.h>

#include "ARGameInstance.generated.h"

class UDataTable;

USTRUCT()
struct FMessageChannelCreatedEventData
{
	GENERATED_BODY()
};

UCLASS()
class ARBASE_API UARGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	static NotNullPtr<UARGameInstance> GetInstance(NotNullPtr<const UObject> outer);

public:
	ar::MessagingManager& GetMessagingManager() { return MessagingManager; }

public:
	UFUNCTION(NetMulticast, Reliable)
	void RPC_Multicast_MessageChannelCreated(FMessageChannelCreatedEventData data);
	

protected:
	virtual void OnStart() override;
	virtual void Shutdown() override;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> WidgetConfigData;

private:
	ar::MessagingManager MessagingManager;
};
