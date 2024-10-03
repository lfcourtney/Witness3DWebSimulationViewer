#include "FTimeStamp.h"


FTimeStamp::FTimeStamp()
{
}


/**
 * @brief Add a unit of time based of the format
 * @param UnitValue 
 * @param Format 
 */
void FTimeStamp::AddUnit(float UnitValue, ETimeUnitFormat Format)
{
	float whole;
	float decimal = std::modf(UnitValue, &whole);
	UnitValue -= decimal;
	switch (Format)
	{
	case ETimeUnitFormat::Years:
		Year += UnitValue;
		Month += std::round(12 * decimal);
		break;
	case ETimeUnitFormat::Months:
		Month += UnitValue;
		Day += std::round(GetMonthMultiplier(Month) * decimal);
		break;
	case ETimeUnitFormat::Days:
		Day += UnitValue;
		Hour += std::round(HourFormat * decimal);
		break;
	case ETimeUnitFormat::Hours:
		Hour += UnitValue;
		Minute += std::round(60 * decimal);
		break;
	case ETimeUnitFormat::Minutes:
		Minute += UnitValue;
		Second += std::round(60 * decimal);
		break;
	case ETimeUnitFormat::Seconds:
		Second += UnitValue;
		MiliSeconds += 1000 * decimal;
		break;
	case ETimeUnitFormat::MiliSeconds:
		MiliSeconds += UnitValue;
		break;
	}
	RoundTimeStamp();
		
}

/**
 * @brief Get de days of the Month
 * @param CurrentMonth 
 * @return 
 */
int FTimeStamp::GetMonthMultiplier(int CurrentMonth)
{
	switch (CurrentMonth)
	{
	case 1:
		return 31;
	case 2:
		return 28;
	case 3:
		return 31;
	case 4:
		return 30;
	case 5:
		return 31;
	case 6:
		return 30;
	case 7:
		return 31;
	case 8:
		return 31;
	case 9:
		return 30;
	case 10:
		return 31;
	case 11:
		return 30;
	case 12:
		return 31;
	}
	return 0;
}


/**
 * @brief Roundup the this TimeStamp
 */
void FTimeStamp::RoundTimeStamp()
{
	if (MiliSeconds >= 1000)
	{
		Second += 1;
		MiliSeconds = 0;
	}
	if (Second > 59)
	{
		Second = 0;
		Minute += 1;
	}
	if (Minute > 59)
	{
		Minute = 0;
		Hour += 1;
	}
	if (Hour >= 24)
	{
		Hour = 0;
		Day += 1;
	}
	if (Day > GetMonthMultiplier(Month))
	{
		Day = 1;
		Month += 1;
	}
	if (Month > 12)
	{
		Month = 1;
		Year += 1;
	}
}

void FTimeStamp::NegativeRoundStamp()
{
	if (Second < 1)
	{
		Day -= 1;
		Second = 0;
	}
	if (Minute < 1)
	{
		Minute = 0;
		Second = 59;
	}
	if (Hour < 1)
	{
		Minute = 59;
		Hour = 0;
	}
	if (Day < 1)
	{
		Hour = 23;
		Minute = 59;
		Second = 59;
	}
	if (Month < 1)
	{
		Month = 12;
		Day = GetMonthMultiplier(Month);
	}
	if(Year < 1)
	{
		Month = 12;
		Year = 0;
	}
}
