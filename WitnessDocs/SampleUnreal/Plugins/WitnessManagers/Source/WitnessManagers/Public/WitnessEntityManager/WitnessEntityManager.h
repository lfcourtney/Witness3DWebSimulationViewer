// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WitnessEntityTimeSeries.h"
#include "Runtime/XmlParser/Public/XmlParser.h"
#include "WitnessEntityManager.generated.h"

UCLASS(Blueprintable)
class WITNESSMANAGERS_API AWitnessEntityManager : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TMap<FString, UWitnessEntityTimeSeries*> EntityMap = TMap<FString, UWitnessEntityTimeSeries*>();
	UPROPERTY(BlueprintReadWrite)
	int64 Time = 0;
	
	// Sets default values for this actor's properties
	AWitnessEntityManager()
	{
		// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
		PrimaryActorTick.bCanEverTick = true;
	}

	


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
			if(!instanceName.Contains("Entity")) continue;
			if(!EntityMap.Contains(instanceName))
			{
				UWitnessEntityTimeSeries* ts = NewObject<UWitnessEntityTimeSeries>();
				ts->InstanceName = instanceName;
				EntityMap.Add(instanceName,ts);
			}
			EntityMap[instanceName]->ConsumeNode(node);
		}
		for (TTuple<FString, UWitnessEntityTimeSeries*> m : EntityMap)
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
			for (TTuple<FString, UWitnessEntityTimeSeries*> Map : EntityMap)
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
		for (TTuple<FString, UWitnessEntityTimeSeries*> m : EntityMap)
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
		for (TTuple<FString, UWitnessEntityTimeSeries*> m : EntityMap)
		{
			if(m.Value->GetNextTime() == next) m.Value->Step();
		}
		Time = next;
		return Time;
	}

	UFUNCTION(BlueprintCallable)
	TMap<FString, UWitnessEntity*> SetTimeTo(int64 t)
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

		for (TTuple<FString, UWitnessEntityTimeSeries*> m : EntityMap)
		{
			m.Value->StepTo(to);
		}
	}
	
	UFUNCTION(BlueprintCallable)
	TMap<FString, UWitnessEntity*> GetEntitiesAtTime(int64 t)
	{
		TMap<FString, UWitnessEntity*> entMap = TMap<FString, UWitnessEntity*>();
		for (TTuple<FString, UWitnessEntityTimeSeries*> m : EntityMap)
		{
			entMap.Add(m.Key,m.Value->FindEntityAtTime(t));
		}
	
		return entMap;
	}

	UFUNCTION(BlueprintCallable)
	TMap<FString, UWitnessEntity*> GetEntitiesAtCurrentTime()
	{
		TMap<FString, UWitnessEntity*> entMap = TMap<FString, UWitnessEntity*>();
		for (TTuple<FString, UWitnessEntityTimeSeries*> m : EntityMap)
		{
			entMap.Add(m.Key,m.Value->GetCurrent());
		}
	
		return entMap;
	}

	UFUNCTION(BlueprintCallable)
	TMap<FString, UWitnessEntity*> GetEntitiesAtCurrentTimeInterpolated()
	{
		TMap<FString, UWitnessEntity*> entMap = TMap<FString, UWitnessEntity*>();
		for (TTuple<FString, UWitnessEntityTimeSeries*> m : EntityMap)
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
