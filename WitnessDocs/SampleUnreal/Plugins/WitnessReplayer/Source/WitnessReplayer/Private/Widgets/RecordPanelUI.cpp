// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/RecordPanelUI.h"

void URecordPanelUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	RecordButton->OnClicked.AddDynamic(this, &URecordPanelUI::OnRecord);
}

void URecordPanelUI::OnRecord()
{
	if(!bIsOpened)
		return;

	bIsRecording = !bIsRecording;
	
	OnRecording();

	if(bIsRecording)
		VideoControls->TogglePause(true);
}

void URecordPanelUI::PauseSimulation()
{
	VideoControls->TogglePause(true);
}
