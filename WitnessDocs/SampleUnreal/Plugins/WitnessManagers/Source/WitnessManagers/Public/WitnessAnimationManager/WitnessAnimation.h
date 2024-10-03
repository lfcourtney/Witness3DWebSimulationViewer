#pragma once
#include "WitnessAnimationFrame.h"
#include "Runtime/XmlParser/Public/XmlParser.h"
#include "WitnessAnimation.generated.h"
UCLASS(Blueprintable)
class WITNESSMANAGERS_API UWitnessAnimation: public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	int CurrentIndex = 0;
	UPROPERTY(BlueprintReadWrite)
	int NextIndex = 0;
	UPROPERTY(BlueprintReadWrite)
	FString InstanceName;
	UPROPERTY(BlueprintReadWrite)
	TArray<UWitnessAnimationFrame*> Entities = TArray<UWitnessAnimationFrame*> ();
	
	void ConsumeNode(FXmlNode* Node)
	{
		UWitnessAnimationFrame* pre;
		if(Entities.Num() == 0 )
		{
			pre = NewObject<UWitnessAnimationFrame>();
			pre->InitZero();
		}
		else
		{
			pre = Entities[Entities.Num()-1];
		}
		UWitnessAnimationFrame* ent = NewObject<UWitnessAnimationFrame>();
		ent->InitPrev(pre, Node);
		/*if(Entities.Num() > 0 && pre->Time == ent->Time) Entities[Entities.Num()-1] = ent;
		else*/ Entities.Add(ent);
	}

	UWitnessAnimationFrame* GetFrameAtTimeInterpolated(int64 time)
	{
		int index = Entities.FindLastByPredicate([time](const UWitnessAnimationFrame* ent)
		{
			return ent->UpdateTime <= time;
		});
		UWitnessAnimationFrame* ent = NewObject<UWitnessAnimationFrame>();
		if(index == INDEX_NONE ) return ent;
		int nextIndex = index+1;
		if(nextIndex >= Entities.Num()) nextIndex = index;
		ent->Interpolate(Entities[index], Entities[nextIndex], time);
		return ent;
	}

	int64 GetDuration()
	{
		int64 duration = 0;
		for (UWitnessAnimationFrame* Entity : Entities)
		{
			if(Entity->UpdateTime > duration) duration = Entity->UpdateTime;
		}
		return duration;
	}

	UWitnessAnimationFrame* GetCurrentInterpolated(int64 t)
	{
		UWitnessAnimationFrame* ent = NewObject<UWitnessAnimationFrame>();
		ent->Interpolate(Entities[CurrentIndex], Entities[NextIndex], t);
		return ent;
	}

	UWitnessAnimationFrame* GetCurrent()
	{
		return Entities[CurrentIndex];
	}

	int64 GetNextTime()
	{
		return Entities[NextIndex]->UpdateTime;
	}

	void Step()
	{
		CurrentIndex = NextIndex;
		SetNext();
	}

	void StepTo(int64 to)
	{
		if(to < Entities[0]->UpdateTime)
		{
			CurrentIndex = 0;
			SetNext();
			return;
		}
		if(to >= Entities[Entities.Num()-1]->UpdateTime)
		{
			CurrentIndex = Entities.Num()-1;
			SetNext();
			return;
		}
		if(to == Entities[CurrentIndex]->UpdateTime) return;
		if(to < Entities[CurrentIndex]->UpdateTime)
		{
			while(to <= Entities[CurrentIndex]->UpdateTime && CurrentIndex > 0) CurrentIndex--;
			SetNext();
			return;
		}
		if(to > Entities[CurrentIndex]->UpdateTime)
		{
			while(to >= Entities[CurrentIndex]->UpdateTime && CurrentIndex < Entities.Num() -1 ) CurrentIndex++;
			if(Entities[CurrentIndex]->UpdateTime!=to) CurrentIndex--;
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
			int64 NextTime = Entities[NextIndex]->UpdateTime;
			while(NextIndex < Entities.Num() -1 && Entities[NextIndex+1]->UpdateTime == NextTime)
			{
				NextIndex++;
			}
		}
	}

	UWitnessAnimationFrame* FindEntityAtTime(int64 time)
	{
		int index = Entities.FindLastByPredicate([time](const UWitnessAnimationFrame* ent)
		{
			return ent->UpdateTime <= time;
		});
		CurrentIndex = index;
		SetNext();

		
		return (index == INDEX_NONE ? NewObject<UWitnessAnimationFrame>() : GetCurrent());
	}
};
