// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbitController.h"

#include "ControllerPawn.h"


// Sets default values
AOrbitController::AOrbitController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AOrbitController::BeginPlay()
{
	Super::BeginPlay();
}

void AOrbitController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftMouse", EInputEvent::IE_Pressed, this, &AOrbitController::OnLeftMouse);
	InputComponent->BindAction("LeftMouse", EInputEvent::IE_Released, this, &AOrbitController::OnLeftMouse);

	InputComponent->BindAction("RightMouse", EInputEvent::IE_Pressed, this, &AOrbitController::OnRightMouse);
	InputComponent->BindAction("RightMouse", EInputEvent::IE_Released, this, &AOrbitController::OnRightMouse);

	InputComponent->BindAxis("MousePitch", this, &AOrbitController::MouseForward);
	InputComponent->BindAxis("MouseYaw", this, &AOrbitController::MouseLeft);

	InputComponent->BindAxis("MouseScroll", this, &AOrbitController::OnMouseScroll);
}

void AOrbitController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	InPawn->bUseControllerRotationPitch = true;
	InPawn->bUseControllerRotationRoll = true;
	InPawn->bUseControllerRotationYaw = true;
	
	AControllerPawn* ControllerPawn = Cast<AControllerPawn>(InPawn);
	
	if (ControllerPawn)
	{
		ArmComp = ControllerPawn->SpringArmComponent;
		CameraComp = ControllerPawn->CameraComponent;
		ArmComp->TargetArmLength = 200;
		CameraComp->bUsePawnControlRotation = false;
	}
}

// Called every frame
void AOrbitController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOrbitController::MouseForward(float Direction)
{
	if (Moving)
	{
		GetPawn()->AddMovementInput(-ControlRotation.RotateVector({Direction, 0, 0}) * FVector{1, 1, 0});
	}
	if (Rotating)
	{
		GetPawn()->AddControllerPitchInput(Direction);
	}
}

void AOrbitController::MouseLeft(float Direction)
{
	if (Moving)
	{
		GetPawn()->AddMovementInput(-ControlRotation.RotateVector({0, Direction, 0}) * FVector{1, 1, 0});
	}
	if (Rotating)
	{
		GetPawn()->AddControllerYawInput(Direction);
	}
}

void AOrbitController::OnLeftMouse()
{
	Moving = !Moving;
}

void AOrbitController::OnRightMouse()
{
	Rotating = !Rotating;
}

void AOrbitController::OnMouseScroll(float Direction)
{
	if (ArmComp)
	{
		ArmComp->TargetArmLength = ArmComp->TargetArmLength - Direction;
	}
}
