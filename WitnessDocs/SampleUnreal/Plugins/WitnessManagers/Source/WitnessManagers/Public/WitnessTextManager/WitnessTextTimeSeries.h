#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WitnessText.h"
#include "WitnessTextTimeSeries.generated.h"

UCLASS(Blueprintable)
class WITNESSMANAGERS_API UWitnessTextTimeSeries : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	FString InstanceName;
	UPROPERTY(BlueprintReadWrite)
	int CurrentIndex = 0;
	UPROPERTY(BlueprintReadWrite)
	int NextIndex = 0;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<UWitnessText*> Entities = TArray<UWitnessText*> ();
	UWitnessTextTimeSeries(){};
	
	// UWitnessEntityTimeSeries(FString instanceName)
	// {
	// 	InstanceName = instanceName;
	// };

	int64 GetDuration()
	{
		int64 duration = 0;
		for (UWitnessText* Entity : Entities)
		{
			if(Entity->Time > duration) duration = Entity->Time;
		}
		return duration;
	}

	void ConsumeNode(FXmlNode* Node)
	{
		UWitnessText* pre;
		if(Entities.Num() == 0 )
		{
			pre = NewObject<UWitnessText>();
			pre->InitZero();
		}
		else
		{
			pre = Entities[Entities.Num()-1];
		}
		UWitnessText* ent = NewObject<UWitnessText>();
		ent->InitPrev(pre, Node);
		/*if(Entities.Num() > 0 && pre->Time == ent->Time) Entities[Entities.Num()-1] = ent;
		else*/ Entities.Add(ent);
	}

	UWitnessText* GetCurrentInterpolated(int64 t)
	{
		UWitnessText* ent = NewObject<UWitnessText>();
		ent->Interpolate(Entities[CurrentIndex], Entities[NextIndex], t);
		return ent;
	}

	UWitnessText* GetCurrent()
	{
		return Entities[CurrentIndex];
	}

	int64 GetNextTime()
	{
		return Entities[NextIndex]->Time;
	}

	void Step()
	{
		CurrentIndex = NextIndex;
		SetNext();
	}

	void StepTo(int64 to)
	{
		if(to < Entities[0]->Time)
		{
			CurrentIndex = 0;
			SetNext();
			return;
		}
		if(to >= Entities[Entities.Num()-1]->Time)
		{
			CurrentIndex = Entities.Num()-1;
			SetNext();
			return;
		}
		if(to == Entities[CurrentIndex]->Time) return;
		if(to < Entities[CurrentIndex]->Time)
		{
			while(to <= Entities[CurrentIndex]->Time && CurrentIndex > 0) CurrentIndex--;
			SetNext();
			return;
		}
		if(to > Entities[CurrentIndex]->Time)
		{
			while(to >= Entities[CurrentIndex]->Time && CurrentIndex < Entities.Num() -1 ) CurrentIndex++;
			if(Entities[CurrentIndex]->Time!=to) CurrentIndex--;
			SetNext();
			return;
		}
	}

	void SetNext()
	{
		NextIndex = CurrentIndex;
		if(NextIndex < Entities.Num() -1)
		{
			NextIndex = NextIndex+1;
			int64 NextTime = Entities[NextIndex]->Time;
			while(NextIndex < Entities.Num() -1 && Entities[NextIndex+1]->Time == NextTime)
			{
				NextIndex++;
			}
		}
	}
	
	UWitnessText* FindEntityAtTime(int64 time)
	{
		int index = Entities.FindLastByPredicate([time](const UWitnessText* ent)
		{
			return ent->Time <= time;
		});
		CurrentIndex = index;
		SetNext();
	
		
		return (index == INDEX_NONE ? NewObject<UWitnessText>() : GetCurrent());
	}


	
};
