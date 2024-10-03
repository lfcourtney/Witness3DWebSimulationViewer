// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WitnessObject.h"
#include "Modules/ModuleManager.h"

static TMap<FString, AWitnessObject*> ObjectMap = TMap<FString, AWitnessObject*>();

static bool ContainsName(FString ObjectName)
{
	return ObjectMap.Contains(ObjectName);
}

static TArray<FString> GetKeys()
{
	TArray<FString> Keys;
	ObjectMap.GetKeys(Keys);
	return Keys;
}

static AWitnessObject* GetWitnessObjectFromName(FString ObjectName)
{
	if(ObjectMap.Contains(ObjectName))
	{
		AWitnessObject* WitnessObject= *ObjectMap.Find(ObjectName);
		if(WitnessObject != nullptr)
		{
			return WitnessObject;
		}
	}
	return nullptr;
}

class FXML_InterpeterModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
};
