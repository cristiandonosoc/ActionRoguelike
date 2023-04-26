#include <ARGame/AI/ARAICharacter.h>

#include <ARBase/NotNullPtr.h>
#include <ARGame/AI/ARAIController.h>
#include <ARGame/Gameplay/Components/ARActionComponent.h>
#include <ARGame/Gameplay/Components/ARAttributeComponent.h>
#include <ARGame/UI/ARWidgetManager.h>

#include <Blueprint/UserWidget.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Perception/PawnSensingComponent.h>

AARAICharacter::AARAICharacter()
{
	INIT_BASE_CLIENT_SERVER_SPLIT();

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
	CLIENT_SERVER_CALL(OnHealthChanged, payload);
}