// Fill out your copyright notice in the Description page of Project Settings.


#include "MultipleViewportController.h"

#include "ControllerPawn.h"
#include "Camera/CameraActor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SpotLightComponent.h"
#include "Engine/SceneCapture2D.h"
#include "Engine/SpotLight.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMultipleViewportController::AMultipleViewportController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMultipleViewportController::BeginPlay()
{
	Super::BeginPlay();
	//
	bUseStaticCamera = true;
	ZoomStep = 100;
}

void AMultipleViewportController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &AMultipleViewportController::MoveForward);
	InputComponent->BindAxis("MoveLeft", this, &AMultipleViewportController::MoveLeft);
	InputComponent->BindAxis("MousePitch", this, &AMultipleViewportController::MousePitch);
	InputComponent->BindAxis("MouseYaw", this, &AMultipleViewportController::MouseYaw);
	InputComponent->BindAxis("MoveUp", this, &AMultipleViewportController::MoveUp);
}

void AMultipleViewportController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AControllerPawn* ControllerPawn = Cast<AControllerPawn>(InPawn);
	ControllerPawn->SpringArmComponent->TargetArmLength = 0;
	ControllerPawn->SpringArmComponent->bUsePawnControlRotation = false;
	ControllerPawn->CameraComponent->bUsePawnControlRotation = false;
	ControllerPawn->bUseControllerRotationPitch = false;
	ControllerPawn->bUseControllerRotationYaw = false;
	ControllerPawn->bUseControllerRotationRoll = false;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);

	for (auto Comp : FoundActors)
	{
		UCameraComponent* CameraComponent = Comp->FindComponentByClass<UCameraComponent>();
		if (CameraComponent && Comp != this)
		{
			Cameras.Add(CameraComponent);
		}
	}
	Cameras.RemoveAt(Cameras.Num() - 1);
	Cameras.RemoveAt(Cameras.Num() - 1);
	UpdateMainCamera();
}

void AMultipleViewportController::SetCameraIndex(const int Index)
{
	CameraIndex = Index;
	UpdateMainCamera();
}

// Called every frame
void AMultipleViewportController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SimulateRotation();
}

void AMultipleViewportController::UpdateMainCamera()
{
	if (Cameras.Num() == 0)
		return;
	MainCamera = Cameras[CameraIndex];
	AControllerPawn* ControllerPawn = Cast<AControllerPawn>(GetPawn());

	ControllerPawn->SpringArmComponent->TargetArmLength = 0;
	ControllerPawn->CameraComponent->SetRelativeRotation(FRotator::ZeroRotator);

	ControllerPawn->SetActorLocation(MainCamera->GetOwner()->GetActorLocation());
	if (!bUseStaticCamera)
	{
		ControllerPawn->Controller->SetControlRotation(MainCamera->GetOwner()->GetActorRotation());
	}
	else
	{
		GetPawn()->SetActorRotation(MainCamera->GetOwner()->GetActorRotation());
	}
}

void AMultipleViewportController::OnLeftMouseButton()
{
	if (const int FutureIndex = CameraIndex + 1; FutureIndex >= Cameras.Num())
	{
		CameraIndex = 0;
	}
	else
	{
		CameraIndex = FutureIndex;
	}
	UpdateMainCamera();
}

void AMultipleViewportController::OnRightMouseButton()
{
	if (const int FutureIndex = CameraIndex - 1; FutureIndex < 0)
	{
		CameraIndex = Cameras.Num() - 1;
	}
	else
	{
		CameraIndex = FutureIndex;
	}
	UpdateMainCamera();
}

void AMultipleViewportController::RotateRight(float Delta)
{
	YawDirection = Delta;
}

void AMultipleViewportController::Zoom(float Delta)
{
	AControllerPawn* ControllerPawn = Cast<AControllerPawn>(GetPawn());
	ControllerPawn->SpringArmComponent->TargetArmLength += Delta;
}

void AMultipleViewportController::RotateUp(float Delta)
{
	PitchDirection = Delta;
}

void AMultipleViewportController::RefreshCameras()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);

	AControllerPawn* ControllerPawn = Cast<AControllerPawn>(GetPawn());

	Cameras.Empty();
	for (auto Component : FoundActors)
	{
		if(Component == this)
		{
			continue;
		}
		UCameraComponent* CameraComponent = Component->FindComponentByClass<UCameraComponent>();

		if (CameraComponent)
		{
			if(Cast<USpringArmComponent>(CameraComponent->GetAttachParent()) != nullptr)
				continue;
			
			if (CameraComponent->GetComponentLocation() == FVector::ZeroVector)
			{
				Component->Destroy();
			}
			else
			{
				Cameras.Add(CameraComponent);
				AActor* Light = GetWorld()->SpawnActor(ASpotLight::StaticClass(), &FVector::ZeroVector, &FRotator::ZeroRotator,
											 FActorSpawnParameters());
				Light->GetRootComponent()->SetMobility(EComponentMobility::Movable);
				Light->SetActorRotation(Component->GetActorRotation());
				Light->SetActorLocation(Component->GetActorLocation());

				Light->AttachToComponent(Component->GetRootComponent(),FAttachmentTransformRules::KeepWorldTransform);

				USpotLightComponent* LightComponent = Light->FindComponentByClass<USpotLightComponent>();
				LightComponent->AttenuationRadius = 5000;
				LightComponent->Intensity = 500000;
				LightComponent->OuterConeAngle = 90;
				LightComponent->SourceRadius = 1500;
			}
		}
	}

	PreviewCamera = Cast<ASceneCapture2D>(GetWorld()->SpawnActor(ASceneCapture2D::StaticClass()));
	UpdateMainCamera();

	if(Cameras.Num() > 0)
	{
		const FVector TempCenter = Cameras[0]->GetOwner()->GetActorLocation();
		Center = FVector(TempCenter.X, TempCenter.Y, 0);
	}
}

void AMultipleViewportController::SetPreviewCamera(int Index)
{
	PreviewCamera->SetActorLocation(Cameras[Index]->GetOwner()->GetActorLocation());
	PreviewCamera->SetActorRotation(Cameras[Index]->GetOwner()->GetActorRotation());
}

void AMultipleViewportController::SetPreviewTexture(UTextureRenderTarget2D* Text)
{
	USceneCaptureComponent2D* SceneCaptureComponent2D = PreviewCamera->FindComponentByClass<USceneCaptureComponent2D>();
	SceneCaptureComponent2D->TextureTarget = Text;
}

void AMultipleViewportController::GoToUpperCamera()
{
	SetCameraIndex(0);
	CameraIndex = -1;
	AControllerPawn* ControllerPawn = Cast<AControllerPawn>(GetPawn());
	ControllerPawn->SetActorLocation(ControllerPawn->GetActorLocation() + FVector::UpVector * 4000);
	if(0 < Cameras.Num())
	{
		GetPawn()->SetActorRotation(Cameras[0]->GetOwner()->GetActorRotation());
	}
	
}

void AMultipleViewportController::ToggleStaticCamera()
{
	UseStaticCamera(!bUseStaticCamera);
}

void AMultipleViewportController::UseStaticCamera(bool bShouldUseStaticCamera)
{
	AControllerPawn* ControllerPawn = Cast<AControllerPawn>(GetPawn());
	bUseStaticCamera = bShouldUseStaticCamera;

	ControllerPawn->SpringArmComponent->bUsePawnControlRotation = !bUseStaticCamera;
	ControllerPawn->CameraComponent->bUsePawnControlRotation = !bUseStaticCamera;
	ControllerPawn->bUseControllerRotationPitch = !bUseStaticCamera;
	ControllerPawn->bUseControllerRotationYaw = !bUseStaticCamera;
	ControllerPawn->bUseControllerRotationRoll = !bUseStaticCamera;

	if (bUseStaticCamera)
	{
		MovementRotationSpeed = 1;
	}
	else
	{
		MovementRotationSpeed = 1500;
		ControllerPawn->Controller->SetControlRotation(GetPawn()->GetActorRotation());
	}
}

void AMultipleViewportController::SimulateRotation() const
{
	AControllerPawn* ControllerPawn = Cast<AControllerPawn>(GetPawn());
	FRotator NewRotation = ControllerPawn->CameraComponent->GetComponentRotation() + FRotator(
		PitchDirection * MovementRotationSpeed, YawDirection * MovementRotationSpeed, 0);
	ControllerPawn->CameraComponent->SetWorldRotation(NewRotation);
}

void AMultipleViewportController::MoveForward(float Direction)
{
	if (!bUseStaticCamera)
	{
		UFloatingPawnMovement* FloatingPawnMovement = Cast<UFloatingPawnMovement>(GetPawn()->GetMovementComponent());
		FloatingPawnMovement->MaxSpeed = MovementRotationSpeed;
		GetPawn()->AddMovementInput(ControlRotation.RotateVector({Direction, 0, 0}) * FVector{1, 1, 0} * MovementRotationSpeed);
	}
}

void AMultipleViewportController::MoveLeft(float Direction)
{
	if (!bUseStaticCamera)
	{
		UFloatingPawnMovement* FloatingPawnMovement = Cast<UFloatingPawnMovement>(GetPawn()->GetMovementComponent());
		FloatingPawnMovement->MaxSpeed = MovementRotationSpeed;
		GetPawn()->AddMovementInput(ControlRotation.RotateVector({0, Direction, 0}) * FVector{1, 1, 0} * MovementRotationSpeed);
	}
}

void AMultipleViewportController::MousePitch(float Delta)
{
	if (!bUseStaticCamera)
	{
		GetPawn()->AddControllerPitchInput(-Delta);
	}
		
}

void AMultipleViewportController::MouseYaw(float Delta)
{
	if (!bUseStaticCamera)
	{
		GetPawn()->AddControllerYawInput(Delta);
	}
		
}

void AMultipleViewportController::MoveUp(float Delta)
{
	if (!bUseStaticCamera)
	{
		UFloatingPawnMovement* FloatingPawnMovement = GetPawn()->FindComponentByClass<UFloatingPawnMovement>();
		FloatingPawnMovement->MaxSpeed = MovementRotationSpeed;
		GetPawn()->AddMovementInput(ControlRotation.RotateVector({0, 0, Delta}) * FVector{0, 0, 1} * MovementRotationSpeed);
	}
}

void AMultipleViewportController::ConsumeUpRotation()
{
	RotateUp(bUseStaticCamera ? MovementRotationSpeed : 0);
}

void AMultipleViewportController::ConsumeDownRotation()
{
	RotateUp(bUseStaticCamera ? MovementRotationSpeed * -1 : 0);
}

void AMultipleViewportController::ConsumeLeftRotation()
{
	RotateRight(bUseStaticCamera ? MovementRotationSpeed * -1 : 0);
}

void AMultipleViewportController::ConsumeRightRotation()
{
	RotateRight(bUseStaticCamera ? MovementRotationSpeed : 0);
}

void AMultipleViewportController::InterruptVerticalRotation()
{
	RotateUp(0);
}

void AMultipleViewportController::InterruptHorizontalRotation()
{
	RotateRight(0);
}

void AMultipleViewportController::ApplyCameraZoomInStep()
{
	if(bUseStaticCamera)
	{
		Zoom(-ZoomStep);
	}
}

void AMultipleViewportController::ApplyCameraZoomOutStep()
{
	if(bUseStaticCamera)
	{
		Zoom(ZoomStep);
	}
}

void AMultipleViewportController::IncreaseRotationSpeed()
{
	MovementRotationSpeed = bUseStaticCamera ? 10 : 5000;
}

void AMultipleViewportController::DecreaseCameraSpeedKey()
{
	MovementRotationSpeed = bUseStaticCamera ? 1 : 1000;
}
