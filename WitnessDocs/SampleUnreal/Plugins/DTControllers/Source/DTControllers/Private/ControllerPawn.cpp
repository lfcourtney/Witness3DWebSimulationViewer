#include "ControllerPawn.h"

#include "GameFramework/Character.h"
#include "GameFramework/FloatingPawnMovement.h"

AControllerPawn::AControllerPawn(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	PawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("Pawn Movement Component");

	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent->SetupAttachment(SpringArmComponent);

	CameraComponent->bUsePawnControlRotation = 1;
}

void AControllerPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AControllerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UPawnMovementComponent* AControllerPawn::GetMovementComponent() const
{
	return PawnMovementComponent;
}

void AControllerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	this->InputComponent = PlayerInputComponent;
}

