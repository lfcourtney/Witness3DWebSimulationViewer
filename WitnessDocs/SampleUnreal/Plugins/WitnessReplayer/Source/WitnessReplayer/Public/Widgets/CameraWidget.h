// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultipleViewportController.h"
#include "Camera/CameraComponent.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "CameraWidget.generated.h"


UCLASS()
class WITNESSREPLAYER_API UCameraWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UBorder* RenderBorder;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* CamButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* RenderImage;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* CameraImage;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* CameraIndexText;

	UPROPERTY(BlueprintReadWrite)
	UCameraComponent* Camera;

	UPROPERTY(BlueprintReadWrite)
	AMultipleViewportController* ViewportController;
	
	UPROPERTY(BlueprintReadWrite)
	int CameraIndex;
	
	//UPROPERTY(BlueprintReadWrite)
	//bool bHasFlippedPosition;

	UPROPERTY(BlueprintReadWrite)
	FVector2D VideoOffset = { 4, 4};
	
	UPROPERTY(BlueprintReadWrite)
	FVector2D BottomRight = { 532, 320};

	UPROPERTY(BlueprintReadWrite)
	FVector2D TopLeft = { 0, 0};

	UFUNCTION(BlueprintCallable)
	void FlipPosition(FVector2D Direction);

	UFUNCTION(BlueprintCallable)
	void LookAtPosition(FVector TargetLocation, bool bIsUpperCamera);

	UFUNCTION(BlueprintCallable)
	void SetCamera(int Index, UCameraComponent* CameraComponent);

	UFUNCTION()
	void ConsumeCamera();

	UFUNCTION()
	void ConsumePreviewCamera();

	UFUNCTION()
	void HidePreviewCamera();
	
	virtual void NativeOnInitialized() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};