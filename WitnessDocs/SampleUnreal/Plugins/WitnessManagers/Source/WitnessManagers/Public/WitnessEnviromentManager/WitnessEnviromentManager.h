#pragma once

#include "XmlFile.h"
#include "Runtime/XmlParser/Public/XmlParser.h"
#include "WitnessEnviromentObject.h"
#include "WitnessExtrude.h"
#include "WitnessSurface.h"
#include "WitnessEnviromentManager.generated.h"

UCLASS(Blueprintable)
class WITNESSMANAGERS_API AWitnessEnviromentManager: public AActor
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadWrite)
	TMap<FString, UWitnessEnviromentObject*> EnviromentMap = TMap<FString, UWitnessEnviromentObject*>();

	AWitnessEnviromentManager(){}

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
			if(Tag=="create")
			{
				FString instanceName = node->GetAttribute("instanceName");
				if(instanceName.Contains("BACKDROP"))
				{
					//Create Extrude
					if(!EnviromentMap.Contains(instanceName))
					{
						UWitnessExtrude* ts = NewObject<UWitnessExtrude>();
						ts->InstanceName = instanceName;
						EnviromentMap.Add(instanceName,ts);
					}
					UWitnessExtrude* Extrude = (UWitnessExtrude*)EnviromentMap[instanceName];
					Extrude->Init(node);
				}
				else if(instanceName.Contains("Floor"))
				{
					//Create Surface
					if(!EnviromentMap.Contains(instanceName))
					{
						UWitnessSurface* ts = NewObject<UWitnessSurface>();
						ts->InstanceName = instanceName;
						EnviromentMap.Add(instanceName,ts);
					}
					UWitnessSurface* Surface = (UWitnessSurface*)EnviromentMap[instanceName];
					Surface->Init(node);
				}
				
			}
		}
		return true;
	}

	UFUNCTION(BlueprintCallable)
	UWitnessExtrude* GetExtrude(FString instanceName)
	{
		if(EnviromentMap.Contains(instanceName))
		{
			if(EnviromentMap[instanceName]->GetClass() == UWitnessExtrude::StaticClass())
			{
				return (UWitnessExtrude*)EnviromentMap[instanceName];
			}
		}
		return nullptr;
	}

	UFUNCTION(BlueprintCallable)
	UWitnessSurface* GetSurface(FString instanceName)
	{
		if(EnviromentMap.Contains(instanceName))
		{
			if(EnviromentMap[instanceName]->GetClass() == UWitnessSurface::StaticClass())
			{
				return (UWitnessSurface*)EnviromentMap[instanceName];
			}
		}
		return nullptr;
	}
};
