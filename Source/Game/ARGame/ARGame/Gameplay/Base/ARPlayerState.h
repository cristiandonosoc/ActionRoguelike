#pragma once

#include <CoreMinimal.h>
#include <GameFramework/PlayerState.h>

#include "ARPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerCreditsChanged, int32, credits);

UCLASS()
class ARGAME_API AARPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	int32 GetCredits() const { return Credits; }

	UFUNCTION(BlueprintCallable)
	int32 ChangeCredits(int32 change);

public:
	UPROPERTY(BlueprintAssignable)
	FOnPlayerCreditsChanged OnPlayerCreditsChanged;

protected:
	UPROPERTY(VisibleAnywhere)
	int32 Credits;
};