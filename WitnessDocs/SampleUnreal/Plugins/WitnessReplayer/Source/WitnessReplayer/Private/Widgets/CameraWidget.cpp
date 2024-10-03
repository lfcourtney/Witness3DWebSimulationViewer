// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CameraWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/KismetMathLibrary.h"

void UCameraWidget::FlipPosition(FVector2D Direction)
{
	Direction.X = Direction.X == 0 ? BottomRight.X : TopLeft.X;
	Direction.Y = Direction.Y == 0 ? BottomRight.Y : TopLeft.Y;
	
	RenderBorder->SetRenderTranslation(Direction);
	RenderImage->SetRenderTranslation(VideoOffset + Direction);
}

void UCameraWidget::LookAtPosition(FVector TargetLocation, bool bIsUpperCamera)
{
	FVector CameraLocation = Camera->GetComponentLocation();
	CameraLocation.Z = 0;
	FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(CameraLocation, TargetLocation);
	CameraImage ->SetRenderTransformAngle(bIsUpperCamera ? 0 : LookRotation.Yaw + 90);
}

void UCameraWidget::SetCamera(int Index, UCameraComponent* CameraComponent)
{
	Camera = CameraComponent;
	CameraIndex = Index;
	CameraIndexText->SetText(FText::AsNumber(Index + 1));
}

void UCameraWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();



	CamButton->OnClicked.AddDynamic(this, &UCameraWidget::ConsumeCamera);
	CamButton->OnHovered.AddDynamic(this, &UCameraWidget::ConsumePreviewCamera);
	CamButton->OnUnhovered.AddDynamic(this, &UCameraWidget::HidePreviewCamera);

	FlipPosition({0, 0});
	
}


void UCameraWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(ViewportController->CameraIndex == CameraIndex)
	{
		SetRenderOpacity(0);
	}
	else
	{
		FVector2D ScreenPosition;
		UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(ViewportController, Camera->GetComponentLocation(), ScreenPosition, true);

		if(UKismetMathLibrary::EqualEqual_Vector2DVector2D(ScreenPosition, {0, 0}, 1))
		{
			SetRenderOpacity(0);
		}
		else
		{
			ScreenPosition -= BottomRight;
			SetRenderTranslation(ScreenPosition);
			SetRenderOpacity(1);
			FlipPosition(FVector2D {ScreenPosition.X > 1060 ? 1.0f : 0.0f, ScreenPosition.Y > 520 ? 1.0f : 0.0f});
			LookAtPosition(ViewportController->Center, ViewportController->CameraIndex <= 0);
		}
	}
}

void UCameraWidget::ConsumeCamera()
{
	ViewportController->SetCameraIndex(CameraIndex);
}

void UCameraWidget::ConsumePreviewCamera()
{
	RenderImage->SetRenderOpacity(1);
	RenderBorder->SetRenderOpacity(1);
	ViewportController->SetPreviewCamera(CameraIndex);
}

void UCameraWidget::HidePreviewCamera()
{
	RenderImage->SetRenderOpacity(0);
	RenderBorder->SetRenderOpacity(0);
}