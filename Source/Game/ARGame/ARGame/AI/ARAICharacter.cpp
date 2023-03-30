// Fill out your copyright notice in the Description page of Project Settings.

#include <ARGame/AI/ARAICharacter.h>

#include <ARBase/NotNullPtr.h>
#include <ARGame/AI/ARAIController.h>
#include <ARGame/Gameplay/Components/ARActionComponent.h>
#include <ARGame/Gameplay/Components/ARAttributeComponent.h>
#include <ARGame/UI/ARWidgetManager.h>
#include <ARGame/UI/Widgets/ARDamagePopupWidget.h>
#include <ARGame/UI/Widgets/AREnemyHealthBarWidget.h>

#include <Blueprint/UserWidget.h>
#include <BrainComponent.h>
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Perception/PawnSensingComponent.h>

// Sets default values
AARAICharacter::AARAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UARAttributeComponent>("Attributes");
	Actions = CreateDefaultSubobject<UARActionComponent>("Actions");
	PawnSenses = CreateDefaultSubobject<UPawnSensingComponent>("PawnSenses");
}

void AARAICharacter::BeginPlay()
{
	Super::BeginPlay();

	PawnSenses->OnSeePawn.AddDynamic(this, &AARAICharacter::OnSeePawn);
	Attributes->OnHealthChanged.AddDynamic(this, &AARAICharacter::OnHealthChanged);
}

void AARAICharacter::OnSeePawn(APawn* pawn)
{
	NotNullPtr ai = Cast<AARAIController>(GetController());
	ai->SetTargetActor(pawn);
}

void AARAICharacter::OnHealthChanged(const FOnHealthChangedPayload& payload)
{
	if (!HealthBarWidget)
	{
		HealthBarWidget = UARWidgetManager::CreateEnemyHealthBarWidget(
			this, Attributes->GetCurrentHealth(), Attributes->GetMaxHealth());
		if (HealthBarWidget)
		{
			// TODO(cdc): Make this translation configurable.
			HealthBarWidget->TranslationOffset = FVector(0, 0, 75);
			HealthBarWidget->AddToViewport();
		}
	}
	else
	{
		HealthBarWidget->CurrentHealth = Attributes->GetCurrentHealth();
		HealthBarWidget->MaxHealth = Attributes->GetMaxHealth();
	}

	if (payload.ActualDelta < 0.0f)
	{
		if (auto* mesh = GetMesh())
		{
			mesh->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
		}

		auto* popup = UARWidgetManager::CreateDamagePopupWidget(this, payload.ActualDelta);
		popup->AddToViewport();
	}

	if (payload.Killed())
	{
		// Stop BT.
		NotNullPtr ai = Cast<AARAIController>(GetController());
		ai->GetBrainComponent()->StopLogic(TEXT("Killed"));

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->DisableMovement();


		// Rag-doll.
		// We make all bones simulate physics.
		if (auto* mesh = GetMesh())
		{
			mesh->SetAllBodiesSimulatePhysics(true);
			mesh->SetCollisionProfileName(TEXT("Ragdoll"));
		}

		HealthBarWidget->RemoveFromParent();

		// Destroy the character after a while.
		SetLifeSpan(10.0f);

		return;
	}

	// If there is an instigator, make the AI react to it.
	if (APawn* instigator = Cast<APawn>(payload.Instigator.Get()))
	{
		if (instigator != this)
		{
			NotNullPtr ai = Cast<AARAIController>(GetController());
			ai->SetTargetActor(instigator);
		}
	}
}

bool AARAICharacter::PerformPrimaryAttack(const AActor& target)
{
	if (!ensure(PrimaryAttackProjectile))
	{
		return false;
	}

	// Get the location where the character is supposed to attack from.
	FVector muzzle_location = GetMesh()->GetSocketLocation("Muzzle_01");
	if (muzzle_location.IsZero())
	{
		return false;
	}

	// We get the direction in which we need to shoot the projectile.
	FVector direction = target.GetActorLocation() - muzzle_location;
	direction.Normalize();

	FRotator rotation = direction.Rotation();

	// Add the spread
	if (PrimaryAttackSpread > 0.0f)
	{
		rotation.Pitch += FMath::RandRange(-PrimaryAttackSpread, PrimaryAttackSpread);
		rotation.Yaw += FMath::RandRange(-PrimaryAttackSpread, PrimaryAttackSpread);
	}

	FTransform spawn_transform = FTransform(rotation, muzzle_location);

	FActorSpawnParameters params = {};
	NotNullPtr pawn = Cast<APawn>(this);
	params.Instigator = pawn;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* proj =
		GetWorld()->SpawnActor<AActor>(PrimaryAttackProjectile.Get(), spawn_transform, params);

	// If the projectile could not be created, we mark the attack as failed.
	if (!proj)
	{
		return false;
	}

	return true;
}