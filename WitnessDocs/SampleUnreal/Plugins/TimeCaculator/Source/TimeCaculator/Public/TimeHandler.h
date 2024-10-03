// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FTimeStamp.h"
#include "ETimeUnitFormat.h"
#include "TimeHandler.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TIMECACULATOR_API UTimeHandler : public UObject
{
	GENERATED_BODY()
public:
	
	inline static TMap<int, FString> MonthNameTable =
	{
		{1, "Jan"},
		{2, "Feb"},
		{3, "Mar"},
		{4, "Apr"},
		{5, "May"},
		{6, "Jun"},
		{7, "Jul"},
		{8, "Aug"},
		{9, "Sep"},
		{10, "Oct"},
		{11, "Nov"},
		{12, "Dec"},
	};

	UPROPERTY(BlueprintReadWrite)
	FTimeStamp StartWorldTimeStamp;
	
	UPROPERTY(BlueprintReadWrite)
	FTimeStamp WorldTimeStamp;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<ETimeUnitFormat> Format;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<ETimeDisplayFormat> Display;

	UFUNCTION(BlueprintCallable)
	FTimeStamp AddUnitToTimeStamp(float UnitValue, FTimeStamp Stamp);

	UFUNCTION(BlueprintCallable)
	static FString TimeStampDateToString(FTimeStamp Stamp);

	UFUNCTION(BlueprintCallable)
	static FString TimeStampTimeToString(FTimeStamp Stamp);

	UFUNCTION(BlueprintCallable)
	static FString GetMonthName(int Month);

	UFUNCTION(BlueprintCallable)
	static FTimeStamp StringToTimeStamp(FString givenString);

	UFUNCTION(BlueprintCallable)
	static FString GetNowDate();

	UFUNCTION(BlueprintCallable)
	void ResetWorldTimeStamp();

	UFUNCTION(BlueprintCallable)
	FTimeStamp SpoolTimeStamp(float TimeAt,FTimeStamp Stamp);

private:
	static FString FormatNumberToString(int N);
	
};
