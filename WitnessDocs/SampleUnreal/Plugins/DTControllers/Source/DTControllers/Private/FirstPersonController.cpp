#include "FirstPersonController.h"
#include "GameFramework/Character.h"

AFirstPersonController::AFirstPersonController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFirstPersonController::BeginPlay()
{
	Super::BeginPlay();
}

void AFirstPersonController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &AFirstPersonController::MoveForward);
	InputComponent->BindAxis("MoveLeft", this, &AFirstPersonController::MoveLeft);
	InputComponent->BindAxis("MousePitch", this, &AFirstPersonController::MousePitch);
	InputComponent->BindAxis("MouseYaw", this, &AFirstPersonController::MouseYaw);
}

void AFirstPersonController::MoveForward(float Direction)
{
	GetPawn()->AddMovementInput(ControlRotation.RotateVector({Direction, 0,0}) * FVector{1, 1, 0});
}

void AFirstPersonController::MoveLeft(float Direction)
{
	GetPawn()->AddMovementInput(ControlRotation.RotateVector({0, Direction,0}) * FVector{1, 1, 0});
}

void AFirstPersonController::MousePitch(float Delta)
{
	GetPawn()->AddControllerPitchInput(-Delta);
}

void AFirstPersonController::MouseYaw(float Delta)
{
	GetPawn()->AddControllerYawInput(Delta);
}
