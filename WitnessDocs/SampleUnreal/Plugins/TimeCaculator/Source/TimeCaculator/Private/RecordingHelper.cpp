// Fill out your copyright notice in the Description page of Project Settings.


#include "RecordingHelper.h"

void URecordingHelper::OpenExplorer(FString FilePath)
{
	FString tempCommand = "explorer " + FilePath;
	char const* Command = TCHAR_TO_ANSI(*tempCommand);
	system(Command);
}
