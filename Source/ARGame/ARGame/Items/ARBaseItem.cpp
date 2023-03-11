#include <ARGame/Items/ARBaseItem.h>

#include <ARBase/NotNullPtr.h>
#include <ARGame/Gameplay/ARGameplayInterface.h>

// Sets default values
AARBaseItem::AARBaseItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if
	// you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AARBaseItem::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AARBaseItem::Tick(float delta)
{
	Super::Tick(delta);
}

namespace
{

bool CheckPrice(NotNullPtr<APawn> interactor, int32 Price)
{
	if (Price <= 0)
	{
		return true;
	}

	// If there is a price, it has to be a credit holder.
	if (!interactor->Implements<UARCreditHolder>())
	{
		return false;
	}

	if (IARCreditHolder::Execute_GetCurrentCredits(interactor) < Price)
	{
		return false;
	}

	return true;
}

} // namespace

bool AARBaseItem::CanUse_Implementation(APawn* interactor)
{
	check(interactor);

	if (!CheckPrice(interactor, Price))
	{
		return false;
	}

	return true;
}

void AARBaseItem::Use_Implementation(APawn* interactor)
{
	check(CanUse(interactor));
	check(interactor);
	
	if (Price > 0)
	{
		bool success = IARCreditHolder::Execute_PayCredits(interactor, Price);
		check(success);
	}
}