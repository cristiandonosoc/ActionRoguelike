#include <ARGame/AI/ARAttributeFunctionLibrary.h>

#include <ARGame/ARAttributeComponent.h>

namespace
{

UARAttributeComponent* GetAttributes(NotNullPtr<AActor> target)
{
	return target->FindComponentByClass<UARAttributeComponent>();
}

} // namespace

bool UARAttributeFunctionLibrary::ApplyDamage(AActor* instigator, AActor* target, float damage)
{
	if (auto* attributes = GetAttributes(target))
	{
		return attributes->ApplyHealthChange(instigator, -damage); // We apply a negative delta.
	}

	return false;
}

bool UARAttributeFunctionLibrary::ApplyDirectionalDamage(AActor* instigator, AActor* target,
														 float damage, const FHitResult& hit)
{
	if (!ApplyDamage(instigator, target, damage))
	{
		return false;
	}

	// From here on, we return true, as we still did damage.

	UPrimitiveComponent* primitive = hit.GetComponent();
	if (primitive && primitive->IsSimulatingPhysics(hit.BoneName))
	{
		primitive->AddImpulseAtLocation(hit.Location, -hit.ImpactNormal * 30000.0f, hit.BoneName);
	}

	return true;
}

bool UARAttributeFunctionLibrary::ApplyHeal(AActor* instigator, AActor* target, float heal)
{
	if (auto* attributes = GetAttributes(target))
	{
		return attributes->ApplyHealthChange(instigator, heal);
	}

	return false;
}