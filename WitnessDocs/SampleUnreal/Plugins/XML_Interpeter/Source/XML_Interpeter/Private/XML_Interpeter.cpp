// Copyright Epic Games, Inc. All Rights Reserved.

#include "XML_Interpeter.h"

#define LOCTEXT_NAMESPACE "FXML_InterpeterModule"

void FXML_InterpeterModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
}

void FXML_InterpeterModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FXML_InterpeterModule, XML_Interpeter)