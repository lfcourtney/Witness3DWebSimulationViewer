#pragma once
#include "WitnessAnimation.h"
#include "Runtime/XmlParser/Public/XmlParser.h"
#include "WitnessAnimationManager.generated.h"

UCLASS(Blueprintable)
class WITNESSMANAGERS_API AWitnessAnimationManager: public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	int64 Time = 0;
	UPROPERTY(BlueprintReadWrite)
	TMap<FString, UWitnessAnimation*> AnimationMap = TMap<FString, UWitnessAnimation*>();
	UFUNCTION(BlueprintCallable)
	bool LoadXMLFile(FString path)
	{
		if(path == "") { path = FPaths::ProjectContentDir() + "/XML/WitnessRun.xml"; }
	
		FXmlFile* XmlFile = new FXmlFile();
		if(!XmlFile->LoadFile(path) || !XmlFile->IsValid()) return false; 

		auto root = XmlFile->GetRootNode();
		TArray<FXmlNode*> Nodes = root->GetChildrenNodes();
		for (FXmlNode* node : Nodes)
		{
			FString instanceName = node->GetAttribute("instanceName");
			//if(!instanceName.Contains("Entity")) continue;
			if(!AnimationMap.Contains(instanceName))
			{
				UWitnessAnimation* ts = NewObject<UWitnessAnimation>();
				ts->InstanceName = instanceName;
				AnimationMap.Add(instanceName,ts);
			}
			AnimationMap[instanceName]->ConsumeNode(node);
		}
		for (TTuple<FString, UWitnessAnimation*> m : AnimationMap)
		{
			//m.Value->SetNext();
		}

		return true;
	}

	UFUNCTION(BlueprintCallable)
	UWitnessAnimationFrame* GetAnimationFrameAtTimeInterpolated(int64 t, FString instanceName)
	{

		if(!AnimationMap.Contains(instanceName)) return NewObject<UWitnessAnimationFrame>();
		UWitnessAnimation* anim = *AnimationMap.Find(instanceName);
		return anim->GetFrameAtTimeInterpolated(t);
	}



	UFUNCTION(BlueprintCallable)
	int64 GetDuration()
	{
		if(dur==-1)
		{
			int64 duration = 0;
			for (TTuple<FString, UWitnessAnimation*> Map : AnimationMap)
			{
				int64 seriesDur = Map.Value->GetDuration();
				if(seriesDur > duration) duration = seriesDur;
			}
			dur = duration;
		}
		return dur;
	}


	UFUNCTION(BlueprintCallable)
	int64 GetNextTime()
	{
		int64 next = dur;
		for (TTuple<FString, UWitnessAnimation*> m : AnimationMap)
		{
			int64 t = m.Value->GetNextTime();
			if(t > Time && t < next) next = t;
		}
		return next;
	}

	UFUNCTION(BlueprintCallable)
	int64 Step()
	{
		int next = GetNextTime();
		for (TTuple<FString, UWitnessAnimation*> m : AnimationMap)
		{
			if(m.Value->GetNextTime() == next) m.Value->Step();
		}
		Time = next;
		return Time;
	}

	UFUNCTION(BlueprintCallable)
	TMap<FString, UWitnessAnimationFrame*> SetTimeTo(int64 t)
	{
		Time = t;
		if(Time > GetDuration()) Time = GetDuration();
		return GetAnimationFramesAtTime(Time);
	}

	UFUNCTION(BlueprintCallable)
	void StepTo( int64 to)
	{
		if(to < 0) to = 0;
		if(to > GetDuration()) to = GetDuration();
	
		Time = to;

		for (TTuple<FString, UWitnessAnimation*> m : AnimationMap)
		{
			m.Value->StepTo(to);
		}
	}
	
	UFUNCTION(BlueprintCallable)
	TMap<FString, UWitnessAnimationFrame*> GetAnimationFramesAtTime(int64 t)
	{
		TMap<FString, UWitnessAnimationFrame*> entMap = TMap<FString, UWitnessAnimationFrame*>();
		for (TTuple<FString, UWitnessAnimation*> m : AnimationMap)
		{
			entMap.Add(m.Key,m.Value->FindEntityAtTime(t));
		}
	
		return entMap;
	}

	UFUNCTION(BlueprintCallable)
	TMap<FString, UWitnessAnimationFrame*> GetAnimationFramesAtCurrentTime()
	{
		TMap<FString, UWitnessAnimationFrame*> entMap = TMap<FString, UWitnessAnimationFrame*>();
		for (TTuple<FString, UWitnessAnimation*> m : AnimationMap)
		{
			entMap.Add(m.Key,m.Value->GetCurrent());
		}
	
		return entMap;
	}

	UFUNCTION(BlueprintCallable)
	TMap<FString, UWitnessAnimationFrame*> GetAnimationFramesAtCurrentTimeInterpolated()
	{
		TMap<FString, UWitnessAnimationFrame*> entMap = TMap<FString, UWitnessAnimationFrame*>();
		for (TTuple<FString, UWitnessAnimation*> m : AnimationMap)
		{
			entMap.Add(m.Key,m.Value->GetCurrentInterpolated(Time));
		}
	
		return entMap;
	}

	UPROPERTY(BlueprintReadWrite)
	int64 dur = -1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override
	{
		Super::BeginPlay();
	}

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override
	{
		Super::Tick(DeltaTime);
	}
	
};
