// Fill out your copyright notice in the Description page of Project Settings.

#include <ARGame/ARTargetDummy.h>

#include <ARGame/ARAttributeComponent.h>

// Sets default values
AARTargetDummy::AARTargetDummy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if
	// you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;

	Attributes = CreateDefaultSubobject<UARAttributeComponent>("Attributes");
	Attributes->OnHealthChanged.AddDynamic(this, &AARTargetDummy::OnHealthChanged);

}

// Called when the game starts or when spawned
void AARTargetDummy::BeginPlay()
{
	Super::BeginPlay();
}
void AARTargetDummy::OnHealthChanged(const FOnHealthChangedPayload& payload)
{
	UE_LOG(LogTemp, Log, TEXT("HEALTH CHANGED: %f"), payload.NewHealth);

	// We only flash on negative hits.
	if (payload.ActualDelta > 0)
	{
		return;
	}

	Mesh->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
}

// Called every frame
void AARTargetDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
