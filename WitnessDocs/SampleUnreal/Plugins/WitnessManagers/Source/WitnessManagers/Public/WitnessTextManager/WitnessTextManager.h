#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WitnessTextTimeSeries.h"
#include "Misc/Paths.h"
#include "Runtime/XmlParser/Public/XmlParser.h"
#include "WitnessTextManager.generated.h"


UCLASS(Blueprintable)
class WITNESSMANAGERS_API AWitnessTextManager : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	TMap<FString, UWitnessTextTimeSeries*> EntityMap = TMap<FString, UWitnessTextTimeSeries*>();
	UPROPERTY(BlueprintReadWrite)
	int64 Time = 0;
	
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
			if(!instanceName.Contains("Variable")) continue; 
			if(!EntityMap.Contains(instanceName))
			{
				UWitnessTextTimeSeries* ts = NewObject<UWitnessTextTimeSeries>();
				ts->InstanceName = instanceName;
				EntityMap.Add(instanceName,ts);
			}
			EntityMap[instanceName]->ConsumeNode(node);
		}
		for (TTuple<FString, UWitnessTextTimeSeries*> m : EntityMap)
		{
			m.Value->SetNext();
		}


		return true;
	}
	
	UFUNCTION(BlueprintCallable)
	int64 GetDuration()
	{
		if(dur==-1)
		{
			int64 duration = 0;
			for (TTuple<FString, UWitnessTextTimeSeries*> Map : EntityMap)
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
		for (TTuple<FString, UWitnessTextTimeSeries*> m : EntityMap)
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
		for (TTuple<FString, UWitnessTextTimeSeries*> m : EntityMap)
		{
			if(m.Value->GetNextTime() == next) m.Value->Step();
		}
		Time = next;
		return Time;
	}

	UFUNCTION(BlueprintCallable)
	TMap<FString, UWitnessText*> SetTimeTo(int64 t)
	{
		Time = t;
		if(Time > GetDuration()) Time = GetDuration();
		return GetEntitiesAtTime(Time);
	}

	UFUNCTION(BlueprintCallable)
	void StepTo( int64 to)
	{
		if(to < 0) to = 0;
		if(to > GetDuration()) to = GetDuration();
	
		Time = to;

		for (TTuple<FString, UWitnessTextTimeSeries*> m : EntityMap)
		{
			m.Value->StepTo(to);
		}
	}
	
	UFUNCTION(BlueprintCallable)
	TMap<FString, UWitnessText*> GetEntitiesAtTime(int64 t)
	{
		TMap<FString, UWitnessText*> entMap = TMap<FString, UWitnessText*>();
		for (TTuple<FString, UWitnessTextTimeSeries*> m : EntityMap)
		{
			entMap.Add(m.Key,m.Value->FindEntityAtTime(t));
		}
	
		return entMap;
	}

	UFUNCTION(BlueprintCallable)
	TMap<FString, UWitnessText*> GetEntitiesAtCurrentTime()
	{
		TMap<FString, UWitnessText*> entMap = TMap<FString, UWitnessText*>();
		for (TTuple<FString, UWitnessTextTimeSeries*> m : EntityMap)
		{
			entMap.Add(m.Key,m.Value->GetCurrent());
		}
	
		return entMap;
	}

	UFUNCTION(BlueprintCallable)
	TMap<FString, UWitnessText*> GetEntitiesAtCurrentTimeInterpolated()
	{
		TMap<FString, UWitnessText*> entMap = TMap<FString, UWitnessText*>();
		for (TTuple<FString, UWitnessTextTimeSeries*> m : EntityMap)
		{
			entMap.Add(m.Key,m.Value->GetCurrentInterpolated(Time));
		}
	
		return entMap;
	}

	UPROPERTY(BlueprintReadWrite)
	int64 dur = -1;

};
