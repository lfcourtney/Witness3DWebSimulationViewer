#pragma once
#include "CoreMinimal.h"
#include "ETimeUnitFormat.h"
#include "FTimeStamp.generated.h"

USTRUCT(BlueprintType)
struct FTimeStamp
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite)
	int HourFormat;

	UPROPERTY(BlueprintReadWrite)
	int Year;

	UPROPERTY(BlueprintReadWrite)
	int Month;

	UPROPERTY(BlueprintReadWrite)
	int Day;

	UPROPERTY(BlueprintReadWrite)
	int Hour;

	UPROPERTY(BlueprintReadWrite)
	int Minute;

	UPROPERTY(BlueprintReadWrite)
	int Second;

	UPROPERTY(BlueprintReadWrite)
	int MiliSeconds;


	FTimeStamp(int Year, int Month, int Day,
		int Hour, int Minute, int Second, int HourFormat)
	{
		this->Year = Year;
		this->Month = Month;
		this->Day = Day;
		this->Hour = Hour;
		this->Minute = Minute;
		this->Second = Second;
		this->HourFormat = HourFormat;
		this->MiliSeconds = 0;
	}
	FTimeStamp();
	
	void AddUnit(float UnitValue, ETimeUnitFormat Format);
	

private:
	static int GetMonthMultiplier(int CurrentMonth);
	void RoundTimeStamp();
	void NegativeRoundStamp();
};

