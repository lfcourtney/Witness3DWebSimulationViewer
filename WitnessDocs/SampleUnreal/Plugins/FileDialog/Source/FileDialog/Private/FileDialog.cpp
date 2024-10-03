// Copyright Epic Games, Inc. All Rights Reserved.

#include "FileDialog.h"

#define LOCTEXT_NAMESPACE "FFileDialogModule"

void FFileDialogModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FString LibraryPath = FPaths::Combine(
		FPaths::ConvertRelativePathToFull(FPaths::ProjectPluginsDir()),
		TEXT("FileDialog"),
		TEXT("Source"),
		TEXT("ThirdParty"),
		TEXT("commdlg32"),
		TEXT("lib"));
    
	CommonDialogHandle = FPlatformProcess::GetDllHandle(*FPaths::Combine(LibraryPath, TEXT("commdlg32.dll")));
}

void FFileDialogModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FPlatformProcess::FreeDllHandle(CommonDialogHandle);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFileDialogModule, FileDialog)