// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/VideoControlWidget.h"


#include "ReplayerLevelScriptActor.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Widgets/CameraWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "GameFramework/GameUserSettings.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"

#include "Kismet/KismetTextLibrary.h"

void UVideoControlWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	MultipleViewportController = Cast<AMultipleViewportController>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AMultipleViewportController::StaticClass()));
	WitnessReplayerActor = Cast<AReplayerLevelScriptActor>(GetWorld()->GetLevelScriptActor())->WitnessReplayerActor;
	InitializeCameraWidgets();

	FastForwardToggle->OnCheckStateChanged.AddDynamic(this, &UVideoControlWidget::FastForward);
	RewindToggle->OnCheckStateChanged.AddDynamic(this, &UVideoControlWidget::Rewind);

	PlaybackSpeedSelection->OnSelectionChanged.AddDynamic(this, &UVideoControlWidget::SelectPlaybackMultiplier);

	CameraSelection->OnOpening.AddDynamic(this, &UVideoControlWidget::UVideoControlWidget::OnCameraListOpen);

	CameraSelection->OnSelectionChanged.AddDynamic(this, &UVideoControlWidget::SetSelectionCamera);
	PauseToggle->OnCheckStateChanged.AddDynamic(WitnessReplayerActor, &AWitnessReplayerActor::SetPaused);
	HelpButton->OnClicked.AddDynamic(this, &UVideoControlWidget::ToggleHelpButton);
	SettingsToggle->OnCheckStateChanged.AddDynamic(this, &UVideoControlWidget::ToggleSettings);
	StaticCameraToggle->OnCheckStateChanged.AddDynamic(this, &UVideoControlWidget::PlayCameraSwitchAnimation);
	StaticCameraToggle->OnCheckStateChanged.AddDynamic(MultipleViewportController,
	                                                   &AMultipleViewportController::UseStaticCamera);

	Slider->OnValueChanged.AddDynamic(this, &UVideoControlWidget::ConsumeSliderInput);

	ZoomInButton->OnPressed.AddDynamic(MultipleViewportController, &AMultipleViewportController::ApplyCameraZoomInStep);
	ZoomOutButton->OnPressed.AddDynamic(MultipleViewportController,
	                                    &AMultipleViewportController::ApplyCameraZoomOutStep);

	UpButton->OnPressed.AddDynamic(MultipleViewportController, &AMultipleViewportController::ConsumeUpRotation);
	DownButton->OnPressed.AddDynamic(MultipleViewportController, &AMultipleViewportController::ConsumeDownRotation);
	LeftButton->OnPressed.AddDynamic(MultipleViewportController, &AMultipleViewportController::ConsumeLeftRotation);
	RightButton->OnPressed.AddDynamic(MultipleViewportController, &AMultipleViewportController::ConsumeRightRotation);

	UpButton->OnReleased.AddDynamic(MultipleViewportController,
	                                &AMultipleViewportController::InterruptVerticalRotation);
	DownButton->OnReleased.AddDynamic(MultipleViewportController,
	                                  &AMultipleViewportController::InterruptVerticalRotation);
	LeftButton->OnReleased.AddDynamic(MultipleViewportController,
	                                  &AMultipleViewportController::InterruptHorizontalRotation);
	RightButton->OnReleased.AddDynamic(MultipleViewportController,
	                                   &AMultipleViewportController::InterruptHorizontalRotation);
}

void UVideoControlWidget::InitializeInputComponent()
{
	Super::InitializeInputComponent();

	InputComponent->BindAction("Exit", IE_Pressed, this, &UVideoControlWidget::Exit);
	InputComponent->BindAction("SwitchStaticCamera", IE_Pressed, MultipleViewportController,
	                           &AMultipleViewportController::ToggleStaticCamera);
	InputComponent->BindAction("LeftMouse", IE_Pressed, this, &UVideoControlWidget::ConsumeJoystickUsage);
	InputComponent->BindAction("LeftMouse", IE_Released, this, &UVideoControlWidget::InterruptJoystickUsage);
	InputComponent->BindAxis("CameraShortCut", this, &UVideoControlWidget::SetSelectionCameraInput);
}

void UVideoControlWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	ConsumeJoystickInput();

	FTimeStamp Stamp = TimeHandler->WorldTimeStamp;

	FText Date = FText::FromString(TimeHandler->TimeStampDateToString(Stamp));
	FText Time = FText::FromString(TimeHandler->TimeStampTimeToString(Stamp));
	FText Decimal = FText::AsNumber(
		UKismetMathLibrary::MinInt64(WitnessReplayerActor->Time, WitnessReplayerActor->Duration) / 1000000);


	DateText->SetText(Date);
	TimeText->SetText(Time);
	DecimalText->SetText(Decimal);

	UpdateSlider();
	UpdateTimeStamp();
}

void UVideoControlWidget::InitializeCameraWidgets()
{
	PlaybackSpeedSelection->SetSelectedOption("Normal");

	MultipleViewportController->RefreshCameras();
	MultipleViewportController->SetPreviewTexture(CameraRenderTarget);

	int Size = MultipleViewportController->Cameras.Num();

	for (int Index = 0; Index < Size; ++Index)
	{
		CameraSelection->AddOption(FString::Printf(TEXT("Camera %d"), Index + 1));
		UCameraWidget* CameraWidget = CreateWidget<UCameraWidget>(GetWorld(), CameraButtonWidgetClass);
		CameraWidget->AddToViewport(2);
		CameraWidget->ViewportController = MultipleViewportController;
		CameraWidget->SetCamera(Index, MultipleViewportController->Cameras[Index]);
		CameraWidget->CamButton->OnClicked.AddDynamic(this, &UVideoControlWidget::ConsumeCameraSelectionIndex);
	}

	CameraSelection->SetSelectedIndex(0);
	MultipleViewportController->MovementRotationSpeed = 1;
}

void UVideoControlWidget::UploadConfigData(TMap<FString, FString> Config)
{
	TMap<FString, ETimeDisplayFormat> FormatTable =
	{
		{"TimeAndDate", ETimeDisplayFormat::TimeAndDate},
		{"TimeOfDay", ETimeDisplayFormat::Time},
		{"Date", ETimeDisplayFormat::Date},
		{"Decimal", ETimeDisplayFormat::Decimal},
	};

	TMap<FString, ETimeUnitFormat> UnitTable =
	{
		{"Years", ETimeUnitFormat::Years},
		{"Months", ETimeUnitFormat::Months},
		{"Days", ETimeUnitFormat::Days},
		{"Hours", ETimeUnitFormat::Hours},
		{"Minutes", ETimeUnitFormat::Minutes},
		{"Seconds", ETimeUnitFormat::Seconds},
		{"MiliSeconds", ETimeUnitFormat::MiliSeconds},
		{"Decimal", ETimeUnitFormat::MiliSeconds},
	};


	TimeHandler = NewObject<UTimeHandler>();

	FString TimeStampString = "";
	ETimeUnitFormat UnitFormat = ETimeUnitFormat::Minutes;
	int HourFormat = 0;
	ETimeDisplayFormat DisplayFormat = ETimeDisplayFormat::TimeAndDate;

	if (Config.Num() > 0)
	{
		TimeStampString = Config["Time"];
		UnitFormat = UnitTable[Config["TimeUnitFormat"]];
		HourFormat = UKismetStringLibrary::Conv_StringToInt(Config["HourFormat"]);
		DisplayFormat = FormatTable[Config["DisplayFormat"]];
	}

	UWidgetAnimation* FormatAnimation = nullptr;

	switch (DisplayFormat)
	{
	case ETimeDisplayFormat::TimeAndDate:
		FormatAnimation = ToTimeAndDateDisplayAnimation;
		break;
	case ETimeDisplayFormat::Time:
		FormatAnimation = ToTimeDisplayAnimation;
		break;
	case ETimeDisplayFormat::Date:
		FormatAnimation = ToDateDisplayAnimation;
		break;
	case ETimeDisplayFormat::Decimal:
		FormatAnimation = ToDecimalDisplayAnimation;
		break;
	}

	if (IsValid(FormatAnimation))
		PlayAnimation(FormatAnimation, 0, 1, EUMGSequencePlayMode::Forward, 10);


	FTimeStamp TempStamp = TimeHandler->StringToTimeStamp(TimeStampString);
	TempStamp.HourFormat = HourFormat;

	TimeHandler->Format = UnitFormat;
	TimeHandler->Display = DisplayFormat;

	TimeHandler->WorldTimeStamp = TempStamp;
	TimeHandler->StartWorldTimeStamp = TempStamp;
}

void UVideoControlWidget::ConsumeSliderInput(float Percentage)
{
	WitnessReplayerActor->StepTo(static_cast<int64>(Percentage * 100) * WitnessReplayerActor->Duration / 100);
	ProgressBar->SetPercent(Percentage);

	TimeHandler->WorldTimeStamp = TimeHandler->SpoolTimeStamp(WitnessReplayerActor->Time / 1000000,
	                                                          TimeHandler->StartWorldTimeStamp);
}


void UVideoControlWidget::UpdateSlider()
{
	if (Slider->HasMouseCapture())
		return;


	float NormalTime = WitnessReplayerActor->Time / 1000000;
	float NormalDurration = WitnessReplayerActor->Duration / 1000000;
	float Percentage = NormalTime / NormalDurration;

	ProgressBar->SetPercent(Percentage);
	Slider->SetValue(Percentage);
}

void UVideoControlWidget::UpdateTimeStamp()
{
	if (Slider->HasMouseCapture())
	{
		OldTimePoint = WitnessReplayerActor->Time;
		return;
	}


	float NormalTime = WitnessReplayerActor->Time / 1000000;
	float NormalPoint = OldTimePoint / 1000000;

	float Increase = NormalTime - NormalPoint;
	if (Increase > 0)
		TimeHandler->WorldTimeStamp = TimeHandler->AddUnitToTimeStamp(Increase, TimeHandler->WorldTimeStamp);
	else
		TimeHandler->WorldTimeStamp = TimeHandler->SpoolTimeStamp(WitnessReplayerActor->Time / 1000000,
		                                                          TimeHandler->StartWorldTimeStamp);

	OldTimePoint = WitnessReplayerActor->Time;
}

void UVideoControlWidget::FastForward(bool bIsChecked)
{
	//RewindToggle->SetCheckedState(bIsChecked ? ECheckBoxState::Unchecked : ECheckBoxState::Checked);
	WitnessReplayerActor->PlaybackMultiplier = WitnessReplayerActor->PlaybackMultiplier < 0.0f
		                                           ? WitnessReplayerActor->PlaybackMultiplier * -1.0f
		                                           : WitnessReplayerActor->PlaybackMultiplier;
	RewindToggle->SetCheckedState(ECheckBoxState::Unchecked);
	FastForwardToggle->SetCheckedState(ECheckBoxState::Checked);
}

void UVideoControlWidget::Rewind(bool bIsChecked)
{
	//RewindToggle->SetCheckedState(bIsChecked ? ECheckBoxState::Unchecked : ECheckBoxState::Checked);
	WitnessReplayerActor->PlaybackMultiplier = WitnessReplayerActor->PlaybackMultiplier > 0.0f
		                                           ? WitnessReplayerActor->PlaybackMultiplier * -1.0f
		                                           : WitnessReplayerActor->PlaybackMultiplier;
	RewindToggle->SetCheckedState(ECheckBoxState::Checked);
	FastForwardToggle->SetCheckedState(ECheckBoxState::Unchecked);
}

void UVideoControlWidget::SelectPlaybackMultiplier(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectedItem == "0.25x")
	{
		WitnessReplayerActor->PlaybackMultiplier = WitnessReplayerActor->PlaybackMultiplier > 0.0f ? 0.25f : -0.25f;
	}
	else if (SelectedItem == "0.5x")
	{
		WitnessReplayerActor->PlaybackMultiplier = WitnessReplayerActor->PlaybackMultiplier > 0.0f ? 0.5f : -0.5f;
	}
	else if (SelectedItem == "Normal")
	{
		WitnessReplayerActor->PlaybackMultiplier = WitnessReplayerActor->PlaybackMultiplier > 0.0f ? 1.0f : -1.0f;
	}
	else if (SelectedItem == "1.5x")
	{
		WitnessReplayerActor->PlaybackMultiplier = WitnessReplayerActor->PlaybackMultiplier > 0.0f ? 1.5f : -1.5f;
	}
	else if (SelectedItem == "2x")
	{
		WitnessReplayerActor->PlaybackMultiplier = WitnessReplayerActor->PlaybackMultiplier > 0.0f ? 2.0f : -2.0f;
	}

	WitnessReplayerActor->PlaybackSpeed = WitnessReplayerActor->DefaultPlaybackSpeed;
}

void UVideoControlWidget::PlayCameraSwitchAnimation(bool bIsStaticCamera)
{
	if (bIsStaticCamera)
	{
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(MultipleViewportController, nullptr, EMouseLockMode::DoNotLock);
	}
	else
	{
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(MultipleViewportController, nullptr,
		                                                  EMouseLockMode::DoNotLock, false);
	}
	MultipleViewportController->bShowMouseCursor = true;
	PlayAnimation(SwitchCameraWidgetAnimation, 0, 1,
	              bIsStaticCamera ? EUMGSequencePlayMode::Reverse : EUMGSequencePlayMode::Forward);

	/*
	if(bIsChecked)
	{
		//JoystickButton->SetRenderOpacity(1);
		//CameraController->MovementRotationSpeed = 1;
		PlayAnimation(SwitchCameraWidgetAnimation, 0, 1, EUMGSequencePlayMode::Reverse);
	}
	else
	{
		//JoystickButton->SetRenderOpacity(0);
		//CameraController->MovementRotationSpeed = 1000;
		PlayAnimation(SwitchCameraWidgetAnimation, 0, 1, EUMGSequencePlayMode::Forward);
	}
	*/
}

void UVideoControlWidget::ConsumeCameraSelectionIndex()
{
	CameraSelection->SetSelectedIndex(MultipleViewportController->CameraIndex);
}

void UVideoControlWidget::SetSelectionCameraInput(float AxisValue)
{
	int32 Index = MultipleViewportController->CameraIndex + UKismetMathLibrary::FTrunc(AxisValue);

	if (MultipleViewportController->Cameras.IsValidIndex(Index))
	{
		MultipleViewportController->SetCameraIndex(Index);
		CameraSelection->SetSelectedIndex(Index);
	}
	else
	{
		MultipleViewportController->GoToUpperCamera();
	}
}

void UVideoControlWidget::SetSelectionCamera(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if(SelectedItem.IsEmpty())
		return;
	
	MultipleViewportController->SetCameraIndex(CameraSelection->GetSelectedIndex());
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(MultipleViewportController, nullptr,
	                                                  EMouseLockMode::LockInFullscreen);
}

FVector2D UVideoControlWidget::ConvertLocationToTopRightAnchor(FVector2D Location)
{
	const FIntPoint ScreenResolution = UGameUserSettings::GetGameUserSettings()->GetScreenResolution();
	Location.X -= ScreenResolution.X - 75;
	Location.Y = UKismetMathLibrary::Abs(Location.Y);
	return Location;
}

void UVideoControlWidget::ConsumeJoystickInput()
{
	if (bUseJoystick)
	{
		FVector2D MousePosition;
		MultipleViewportController->GetMousePosition(MousePosition.X, MousePosition.Y);
		MousePosition = ConvertLocationToTopRightAnchor(MousePosition);
		MousePosition -= UWidgetLayoutLibrary::SlotAsCanvasSlot(JoystickButton)->GetPosition();
		MousePosition = UKismetMathLibrary::ClampAxes2D(MousePosition, -20, 20);
		JoystickButton->SetRenderTranslation(MousePosition);
		JoystickDirection = UKismetMathLibrary::ClampAxes2D(MousePosition, -0.2f, 0.2f);

		MultipleViewportController->RotateRight(UKismetMathLibrary::Abs(MousePosition.X) > 8 ? JoystickDirection.X : 0);
		MultipleViewportController->RotateUp(UKismetMathLibrary::Abs(MousePosition.Y) > 8
			                                     ? JoystickDirection.Y * -1.0f
			                                     : 0);
	}
	else
	{
		JoystickButton->SetRenderTranslation({0, 0});
	}
}

void UVideoControlWidget::ConsumeJoystickUsage()
{
	FVector2D JoystickPosition = UWidgetLayoutLibrary::SlotAsCanvasSlot(JoystickButton)->GetPosition();
	FVector2D MousePosition;
	MultipleViewportController->GetMousePosition(MousePosition.X, MousePosition.Y);
	MousePosition = ConvertLocationToTopRightAnchor(MousePosition);
	bUseJoystick = MultipleViewportController->bUseStaticCamera && UKismetMathLibrary::Distance2D(
		MousePosition, JoystickPosition) > 40;
}

void UVideoControlWidget::InterruptJoystickUsage()
{
	if (MultipleViewportController->bUseStaticCamera)
	{
		bUseJoystick = false;
		MultipleViewportController->RotateRight(0);
		MultipleViewportController->RotateUp(0);
	}
}

void UVideoControlWidget::ToggleSettings(bool bisChecked)
{
	PlayAnimation(ShowSettingsWidgetAnimation, 0, 1,
	              bisChecked ? EUMGSequencePlayMode::Reverse : EUMGSequencePlayMode::Forward);
}

void UVideoControlWidget::ToggleHelpButton()
{
	//if(!WitnessReplayerActor->bIsPaused)
	if (HelpUI != nullptr)
	{
		if (!HelpUI->IsInViewport())
		{
			WitnessReplayerActor->SetPaused(true);
			if (IsValid(HelpUI))
			{
				HelpUI->AddToViewport(2);
			}
			else
			{
				HelpUI = Cast<UHelpUIWidget>(CreateWidget(GetOwningPlayer(), HelpUIClass));
				HelpUI->AddToViewport(2);
				HelpUI->ContinueButton->OnClicked.AddDynamic(this, &UVideoControlWidget::ToggleHelpButton);
				HelpUI->ExitButton->OnClicked.AddDynamic(this, &UVideoControlWidget::ToggleHelpButton);
			}
			WitnessReplayerActor->SetPaused(true);
		}
		else
		{
			HelpUI->RemoveFromParent();
			WitnessReplayerActor->SetPaused(false);
		}
	}
	else
	{
		HelpUI = Cast<UHelpUIWidget>(CreateWidget(GetOwningPlayer(), HelpUIClass));
		HelpUI->AddToViewport(2);
		HelpUI->ContinueButton->OnClicked.AddDynamic(this, &UVideoControlWidget::ToggleHelpButton);
		HelpUI->ExitButton->OnClicked.AddDynamic(this, &UVideoControlWidget::ToggleHelpButton);
		WitnessReplayerActor->SetPaused(true);
	}
}

void UVideoControlWidget::TogglePause(bool Paused)
{
	WitnessReplayerActor->SetPaused(Paused);

	PauseToggle->SetIsChecked(Paused);
}

void UVideoControlWidget::Exit()
{
	FGenericPlatformMisc::RequestExit(false);
}

void UVideoControlWidget::OnCameraListOpen()
{
	
	
	MultipleViewportController->GoToUpperCamera();
		CameraSelection->ClearSelection();
}
