#pragma once
#include "XmlFile.h"
#include "WitnessStatic.h"
#include "WitnessStaticManager.generated.h"

UCLASS(Blueprintable)
class WITNESSMANAGERS_API AWitnessStaticManager: public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	TMap<FString, UWitnessStatic*> StaticMap = TMap<FString, UWitnessStatic*>();
	AWitnessStaticManager(){}


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
			FString Tag = node->GetTag();
			if(Tag=="create" || Tag=="update")
			{
				FString instanceName = node->GetAttribute("instanceName");
				if(!instanceName.Contains("Icon") && !instanceName.Contains("Queue")) continue;
				if(!StaticMap.Contains(instanceName))
				{
					UWitnessStatic* ts = NewObject<UWitnessStatic>();
					ts->InstanceName = instanceName;
					StaticMap.Add(instanceName,ts);
				}
				StaticMap[instanceName]->Init(node);
			}
		}
		return true;
	}

	UFUNCTION(BlueprintCallable)
	UWitnessStatic* GetStatic(FString instanceName)
	{
		if(StaticMap.Contains(instanceName)) return StaticMap[instanceName];
		return NewObject<UWitnessStatic>();
	}
};
