#pragma once

#include "Runtime/XmlParser/Public/XmlParser.h"
#include "WitnessAnimationFrame.generated.h"
UCLASS(Blueprintable)
class WITNESSMANAGERS_API UWitnessAnimationFrame : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	int64 StartTime;
	UPROPERTY(BlueprintReadWrite)
	int64 UpdateTime;
	UPROPERTY(BlueprintReadWrite)
	int64 StopTime;
	UPROPERTY(BlueprintReadWrite)
	int64 Duration;
	UPROPERTY(BlueprintReadWrite)
	int64 RepeatCount;
	UPROPERTY(BlueprintReadWrite)
	FString InstanceName;
	UPROPERTY(BlueprintReadWrite)
	FString Name;
	UPROPERTY(BlueprintReadWrite)
	bool Started;

	void InitZero()
	{
		StartTime = 0;
		UpdateTime = 0;
		StopTime = 0;
		Duration = 0;
		RepeatCount = 0;
		InstanceName = "";
		Name = "";
		Started = false;
	}

	void InitPrev(UWitnessAnimationFrame* Prev)
	{
		StartTime = Prev->StartTime;
		UpdateTime = Prev->UpdateTime;
		StopTime = Prev->StopTime;
		Duration = Prev->Duration;
		RepeatCount = Prev->RepeatCount;
		InstanceName = Prev->InstanceName;
		Name = Prev->Name;
		Started = Prev->Started;
	}

	void InitPrev(UWitnessAnimationFrame* prev, FXmlNode* Node)
	{
		InitPrev(prev);
		FString Convert = Node->GetAttribute("time");
		Convert = Convert.Replace(TEXT("."),TEXT(""));
		UpdateTime = FCString::Atoi64(*Convert);
		InstanceName = Node->GetAttribute("instanceName");
		Name = Node->GetAttribute("name");

		FString tag = Node->GetTag();
		if(tag == "animationStart")
		{
			prev->Duration = UpdateTime - prev->UpdateTime;
			RepeatCount = FCString::Atoi64(*Node->GetAttribute("name"));
			Duration = FCString::Atoi64(*Node->GetAttribute("duration").Replace(TEXT("."),TEXT("")));
			StartTime = UpdateTime;
			Started = true;
		}
		if(tag == "animationStop")
		{
			StopTime = UpdateTime;
			Started = false;
		}
	}

	float lerp(float a, float b, float f)
	{
		return a * (1.0-f) + b * f;
	}

	void Interpolate(UWitnessAnimationFrame* A, UWitnessAnimationFrame* B, int64 timeAt)
	{
		float f = (B->UpdateTime==A->UpdateTime) ? 0 : ((float)(timeAt-A->UpdateTime)) / (B->UpdateTime-A->UpdateTime);
		InitPrev(A);
		UpdateTime = lerp(A->UpdateTime, B->UpdateTime, f);
	}
};
