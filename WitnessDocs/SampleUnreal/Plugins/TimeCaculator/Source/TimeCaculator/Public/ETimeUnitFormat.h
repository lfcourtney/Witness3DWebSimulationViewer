#pragma once

UENUM()
enum ETimeUnitFormat
{
	MiliSeconds UMETA(DisplayName = "MiliSeconds"),
	Seconds		UMETA(DisplayName = "Seconds"),
	Minutes		UMETA(DisplayName = "Minutes"),
	Hours		UMETA(DisplayName = "Hours"),
	Days		UMETA(DisplayName = "Days"),
	Months		UMETA(DisplayName = "Months"),
	Years		UMETA(DisplayName = "Years"),
	
};

UENUM()
enum ETimeDisplayFormat
{
	TimeAndDate	UMETA(DisplayName = "TimeAndDate"),
	Time UMETA(DisplayName = "Time"),
	Date UMETA(DisplayName = "Date"),
	Decimal UMETA(DisplayName = "Decimal"),
};
