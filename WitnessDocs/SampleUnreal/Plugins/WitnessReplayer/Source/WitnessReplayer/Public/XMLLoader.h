// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/FileManagerGeneric.h"
#include "XMLLoader.generated.h"

/**
 * 
 */
UCLASS()
class WITNESSREPLAYER_API UXMLLoader : public UObject
{
	GENERATED_BODY()

public:
	TMap<FString,FString> LoadFile();

	TMap<FString,FString>  LoadConfig(FString configFilePath);
};
