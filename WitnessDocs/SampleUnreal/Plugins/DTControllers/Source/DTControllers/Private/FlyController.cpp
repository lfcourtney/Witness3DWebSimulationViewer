#include "FlyController.h"

AFlyController::AFlyController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFlyController::BeginPlay()
{
	Super::BeginPlay();
}

void AFlyController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &AFlyController::MoveForward);
	InputComponent->BindAxis("MoveLeft", this, &AFlyController::MoveLeft);
	InputComponent->BindAxis("MousePitch", this, &AFlyController::MousePitch);
	InputComponent->BindAxis("MouseYaw", this, &AFlyController::MouseYaw);
}

void AFlyController::MoveForward(float Direction)
{
	GetPawn()->AddMovementInput(ControlRotation.RotateVector({Direction, 0,0}) * FVector{1, 1, 0});
}

void AFlyController::MoveLeft(float Direction)
{
	GetPawn()->AddMovementInput(ControlRotation.RotateVector({0, Direction,0}) * FVector{1, 1, 0});
}

void AFlyController::MousePitch(float Delta)
{
	GetPawn()->AddControllerPitchInput(-Delta);
}

void AFlyController::MouseYaw(float Delta)
{
	GetPawn()->AddControllerYawInput(Delta);
}
