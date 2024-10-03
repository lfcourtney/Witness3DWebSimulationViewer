// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RecordingHelper.generated.h"

/**
 * 
 */
UCLASS()
class TIMECACULATOR_API URecordingHelper : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	static void OpenExplorer(FString FilePath);
};
