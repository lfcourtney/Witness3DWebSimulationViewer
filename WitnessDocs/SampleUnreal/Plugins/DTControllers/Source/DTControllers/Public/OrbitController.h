// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "OrbitController.generated.h"

UCLASS()
class DTCONTROLLERS_API AOrbitController : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AOrbitController();

	UPROPERTY(BlueprintReadOnly)
	bool Moving;

	UPROPERTY(BlueprintReadOnly)
	bool Rotating;

	UPROPERTY(BlueprintReadOnly)
	FVector CurrentLockMousePosition;

	UPROPERTY(BlueprintReadOnly)
	UCameraComponent* CameraComp;

	UPROPERTY(BlueprintReadOnly)
	USpringArmComponent* ArmComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Allows the PlayerController to set up custom input bindings. */
	virtual void SetupInputComponent() override;

	virtual  void OnPossess(APawn* InPawn) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Called when the MouseForward input binding is triggered. */
	virtual void MouseForward(float Direction);

	/** Called when the MouseLeft input binding is triggered. */
	virtual void MouseLeft(float Direction);

	/** Called when the Left mouse input action is triggered. */
	virtual void OnLeftMouse();

	/** Called when the Left mouse input action is triggered. */
	virtual void OnRightMouse();

	/** Called when the Mouse Scrool input binding is triggered. */
	virtual void OnMouseScroll(float Direction);
};


