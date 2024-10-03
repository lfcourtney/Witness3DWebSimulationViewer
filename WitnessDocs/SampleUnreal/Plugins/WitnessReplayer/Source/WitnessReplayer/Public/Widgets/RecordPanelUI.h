// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VideoControlWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "RecordPanelUI.generated.h"

/**
 * 
 */
UCLASS()
class WITNESSREPLAYER_API URecordPanelUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY()
	
	UVideoControlWidget* VideoControls;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bIsOpened;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bIsRecording;
	
	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
		UButton* RecordButton;

	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnRecord();

	UFUNCTION(BlueprintImplementableEvent)
	void OnRecording();

	UFUNCTION(BlueprintCallable)
	void PauseSimulation();
};
