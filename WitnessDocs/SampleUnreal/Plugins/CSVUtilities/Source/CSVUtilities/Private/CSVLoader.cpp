#include "CSVLoader.h"
#include <Runtime/Core/Public/Misc/Paths.h>
#include <Runtime/Core/Public/HAL/PlatformFilemanager.h>

FString UCSVLoader::LoadFileToString(FString FileSubPath)
{

	FString Result;

	if(FFileHelper::LoadFileToString(Result, *FPaths::Combine(FPaths::LaunchDir(), FileSubPath)))
	{
		UE_LOG(LogTemp, Error, TEXT("Using Build Content"))
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FPaths::Combine(FPaths::LaunchDir(), FileSubPath));
	}
	else if(FFileHelper::LoadFileToString(Result, *FPaths::Combine(FPaths::ProjectContentDir(), FileSubPath)))
	{
		UE_LOG(LogTemp, Error, TEXT("Using Project Content"))
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FPaths::Combine(FPaths::ProjectContentDir(), FileSubPath));
	}
	return Result;
}

TArray<FString> UCSVLoader::LoadFileToStringArray(FString FileSubPath)
{
	TArray<FString> Result;

	if(FFileHelper::LoadFileToStringArray(Result, *FPaths::Combine(FPaths::LaunchDir(), FileSubPath)))
	{
		UE_LOG(LogTemp, Error, TEXT("Using Build Content"))
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FPaths::Combine(FPaths::LaunchDir(), FileSubPath));
	}
	else if(FFileHelper::LoadFileToStringArray(Result, *FPaths::Combine(FPaths::ProjectContentDir(), FileSubPath)))
	{
		UE_LOG(LogTemp, Error, TEXT("Using Project Content"))
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FPaths::Combine(FPaths::ProjectContentDir(), FileSubPath));
	}
	return Result;
}