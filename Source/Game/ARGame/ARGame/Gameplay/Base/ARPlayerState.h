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
	AARPlayerState();

public:
	// INTERFACE_BEGIN(AActor)
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& props) const override;
	// INTERFACE_END(AActor)

public:
	UFUNCTION(BlueprintCallable)
	int32 GetCredits() const { return Credits; }

	UFUNCTION(BlueprintCallable)
	int32 ChangeCredits(int32 change);

protected:
	UFUNCTION()
	void OnRep_Credits(int32 old_credits);

public:
	UPROPERTY(BlueprintAssignable)
	FOnPlayerCreditsChanged OnPlayerCreditsChanged;

protected:
	UPROPERTY(ReplicatedUsing = "OnRep_Credits", VisibleAnywhere)
	int32 Credits;
};