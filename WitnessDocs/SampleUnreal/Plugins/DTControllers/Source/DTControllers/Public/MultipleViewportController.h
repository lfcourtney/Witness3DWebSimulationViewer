// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Engine/SceneCapture2D.h"
#include "MultipleViewportController.generated.h"

UCLASS()
class DTCONTROLLERS_API AMultipleViewportController : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMultipleViewportController();

	UPROPERTY(BlueprintReadOnly)
	TArray<UCameraComponent*> Cameras;

	UPROPERTY(BlueprintReadOnly)
	UCameraComponent* MainCamera;

	UPROPERTY(BlueprintReadOnly)
	ASceneCapture2D* PreviewCamera;

	UPROPERTY(BlueprintReadOnly)
	int CameraIndex;

	UPROPERTY(BlueprintReadOnly)
	bool bUseStaticCamera;

	float YawDirection;

	float PitchDirection;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MovementRotationSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ZoomStep;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FVector Center;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;

public:
	UFUNCTION(BlueprintCallable)
	void SetCameraIndex(const int Index);
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateMainCamera();

	UFUNCTION(BlueprintCallable)
	void OnLeftMouseButton();

	UFUNCTION(BlueprintCallable)
	void OnRightMouseButton();

	UFUNCTION(BlueprintCallable)
	void RotateRight(float Delta);

	UFUNCTION(BlueprintCallable)
	void Zoom(float Delta);

	UFUNCTION(BlueprintCallable)
	void RotateUp(float Delta);

	UFUNCTION(BlueprintCallable)
	void RefreshCameras();

	UFUNCTION(BlueprintCallable)
	void SetPreviewCamera(int Index);

	UFUNCTION(BlueprintCallable)
	void SetPreviewTexture(UTextureRenderTarget2D* Text);

	UFUNCTION(BlueprintCallable)
	void GoToUpperCamera();

	UFUNCTION(BlueprintCallable)
	void ToggleStaticCamera();
	
	UFUNCTION(BlueprintCallable)
	void UseStaticCamera(bool bShouldUseStaticCamera);

	void SimulateRotation() const;

	void MoveForward(float Direction);

	void MoveLeft(float Direction);

	void MousePitch(float Delta);

	void MouseYaw(float Delta);

	void MoveUp(float Delta);

	UFUNCTION()
	void ConsumeUpRotation();

	UFUNCTION()
	void ConsumeDownRotation();

	UFUNCTION()
	void ConsumeLeftRotation();

	UFUNCTION()
	void ConsumeRightRotation();

	UFUNCTION()
	void InterruptVerticalRotation();

	UFUNCTION()
	void InterruptHorizontalRotation();

	UFUNCTION()
	void ApplyCameraZoomInStep();

	UFUNCTION()
	void ApplyCameraZoomOutStep();

	UFUNCTION()
	void IncreaseRotationSpeed();

	UFUNCTION()
	void DecreaseCameraSpeedKey();
};
