// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeHandler.h"

#include <string>

FTimeStamp UTimeHandler::AddUnitToTimeStamp(float UnitValue, FTimeStamp Stamp)
{
	Stamp.AddUnit(UnitValue, Format);
	return Stamp;
}

FString UTimeHandler::TimeStampDateToString(FTimeStamp Stamp)
{
	FString TimeStampString;
	TimeStampString.Append(GetMonthName(Stamp.Month));
	TimeStampString.Append(" ");
	TimeStampString.Append(FormatNumberToString(Stamp.Day));
	TimeStampString.Append(" ");
	TimeStampString.Append(FString::FromInt(Stamp.Year));
	return TimeStampString;
}

FString UTimeHandler::TimeStampTimeToString(FTimeStamp Stamp)
{
	FString TimeStampString;
	bool ApplyTimeText = false;
	FString TimeText = "AM";
	
	if(Stamp.HourFormat == 12)
	{
		ApplyTimeText = true;
		if(Stamp.Hour > 12)
		{
			int TempHour = Stamp.Hour - 12;
			TimeStampString.Append(FormatNumberToString(TempHour));

			TimeText = "PM";
		}
		else
		{
			TimeStampString.Append(FormatNumberToString(Stamp.Hour));
		}
	}
	else
	{
		TimeStampString.Append(FormatNumberToString(Stamp.Hour));
	}
	
	TimeStampString.Append(":");
	TimeStampString.Append(FormatNumberToString(Stamp.Minute));
	TimeStampString.Append(":");
	TimeStampString.Append(FormatNumberToString(Stamp.Second));

	if(ApplyTimeText)
	{
		TimeStampString.Append(" ");
		TimeStampString.Append(TimeText);
	}
	
	return TimeStampString;
}

FString UTimeHandler::GetMonthName(int Month)
{
	if (MonthNameTable.Contains(Month))
		return MonthNameTable[Month];
	else
		return  "NAN";
}

FTimeStamp UTimeHandler::StringToTimeStamp(FString givenString)
{
	const int Length = givenString.Len();
	if (Length <= 0)
		return FTimeStamp();

	FString ConvertString;
	ConvertString.AppendChar(givenString[0]);
	ConvertString.AppendChar(givenString[1]);
	ConvertString.AppendChar(givenString[2]);
	ConvertString.AppendChar(givenString[3]);

	const int Year = std::stoi(*ConvertString);

	ConvertString = "";
	ConvertString.AppendChar(givenString[5]);
	ConvertString.AppendChar(givenString[6]);

	const int Month = std::stoi(*ConvertString);

	ConvertString = "";

	ConvertString.AppendChar(givenString[8]);
	ConvertString.AppendChar(givenString[9]);

	const int Day = std::stoi(*ConvertString);

	ConvertString = "";

	ConvertString.AppendChar(givenString[11]);
	ConvertString.AppendChar(givenString[12]);

	const int Hour = std::stoi(*ConvertString);

	ConvertString = "";

	ConvertString.AppendChar(givenString[14]);
	ConvertString.AppendChar(givenString[15]);

	const int Minute = std::stoi(*ConvertString);

	ConvertString = "";

	ConvertString.AppendChar(givenString[17]);
	ConvertString.AppendChar(givenString[18]);

	const int Second = std::stoi(*ConvertString);

	return FTimeStamp(Year, Month, Day, Hour, Minute, Second, 12);
}

FString UTimeHandler::GetNowDate()
{
	FDateTime Now = FDateTime::Now();
	return Now.ToIso8601();
}

void UTimeHandler::ResetWorldTimeStamp()
{
	WorldTimeStamp = StartWorldTimeStamp;
}

FTimeStamp UTimeHandler::SpoolTimeStamp(float TimeAt, FTimeStamp Stamp)
{
	float Whole;
	float Decimal = std::modf(TimeAt, &Whole);

	for (int i = 0; i < Whole; i++)
	{
		Stamp.AddUnit(1, Format);
	}
	Stamp.AddUnit(Decimal, Format);

	return Stamp;
}

FString UTimeHandler::FormatNumberToString(int N)
{
	if (N >= 10)
		return FString::FromInt(N);

	FString ReturnString = "0";
	ReturnString.Append(FString::FromInt(N));
	return ReturnString;
}
