// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraWidget.h"
#include "HelpUIWidget.h"
#include "WitnessReplayerActor.h"
#include "MultipleViewportController.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/ProgressBar.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "TimeHandler.h"

#include "VideoControlWidget.generated.h"

UCLASS()
class WITNESSREPLAYER_API UVideoControlWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/* Witness Replayer */
	UPROPERTY(EditAnywhere, Category="Witness Replayer")
	TSoftObjectPtr<UWorld> ExitLevel;

	UPROPERTY(VisibleAnywhere, Category="Witness Replayer")
	AWitnessReplayerActor* WitnessReplayerActor;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UCheckBox* SettingsToggle;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* ShowSettingsWidgetAnimation;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* ToTimeAndDateDisplayAnimation;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* ToTimeDisplayAnimation;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* ToDateDisplayAnimation;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* ToDecimalDisplayAnimation;
	
	/* Camera Control */
	UPROPERTY(EditAnywhere, Category="Camera")
	TSubclassOf<UCameraWidget> CameraButtonWidgetClass;

	UPROPERTY(EditAnywhere, Category="Camera Control")
	UTextureRenderTarget2D* CameraRenderTarget;
	
	UPROPERTY(VisibleAnywhere, Category="Camera Control")
	AMultipleViewportController* MultipleViewportController;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UComboBoxString* CameraSelection;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UCheckBox* StaticCameraToggle;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* SwitchCameraWidgetAnimation;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* DateText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* TimeText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* DecimalText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UProgressBar* ProgressBar;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	USlider* Slider;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UCheckBox* PauseToggle;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UCheckBox* FastForwardToggle;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UCheckBox* RewindToggle;

	/* Joystick Control */
	UPROPERTY(VisibleAnywhere, Category="Joystick Control")
	bool bUseJoystick;

	UPROPERTY(VisibleAnywhere, Category="Joystick Control")
	FVector2D JoystickDirection;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* UpButton;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* DownButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* LeftButton;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* RightButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* ZoomInButton;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* ZoomOutButton;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* JoystickButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UUserWidget> HelpUIClass;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UHelpUIWidget* HelpUI;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* HelpButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UComboBoxString* PlaybackSpeedSelection;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UTimeHandler* TimeHandler;

	UPROPERTY()
	int64 OldTimePoint;
	
	
	virtual void NativeOnInitialized() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	virtual void InitializeInputComponent() override;

	
	UFUNCTION()
	void InitializeCameraWidgets();

	UFUNCTION()
	void UploadConfigData(TMap<FString,FString> Config);

	
	UFUNCTION()
    void ConsumeSliderInput(float Percentage);

	UFUNCTION()
	void UpdateSlider();

	UFUNCTION()
	void UpdateTimeStamp();

	UFUNCTION()
	void FastForward(bool bIsChecked);
   	
	UFUNCTION()
	void Rewind(bool bIsChecked);

	
	UFUNCTION()
	void SelectPlaybackMultiplier(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void ConsumeCameraSelectionIndex();

	UFUNCTION()
	void SetSelectionCameraInput(float AxisValue);
	
	UFUNCTION()
  	void SetSelectionCamera(FString SelectedItem, ESelectInfo::Type SelectionType);
  	
  	UFUNCTION()
  	void PlayCameraSwitchAnimation(bool bIsStaticCamera);

	
  	UFUNCTION()
  	FVector2D ConvertLocationToTopRightAnchor(FVector2D Location);
  	
  	UFUNCTION()
  	void ConsumeJoystickInput();
  	
  	UFUNCTION()
  	void ConsumeJoystickUsage();
  
  	UFUNCTION()
  	void InterruptJoystickUsage();
  	
  	UFUNCTION()
  	void ToggleSettings(bool bIsChecked);

	UFUNCTION()
	void ToggleHelpButton();

	UFUNCTION()
	void TogglePause(bool Paused);
  	
  	UFUNCTION()
  	void Exit();

	UFUNCTION()
	void OnCameraListOpen();
  };






