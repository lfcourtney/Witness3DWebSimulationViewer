#pragma once
#include "WitnessPath.h"
#include "WitnessPathManager.generated.h"
UCLASS(Blueprintable)
class WITNESSMANAGERS_API AWitnessPathManager: public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	TMap<FString, UWitnessPath*> PathMap = TMap<FString, UWitnessPath*>();
	AWitnessPathManager(){}


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
			if(!instanceName.Contains("Path")) continue;
			if(!PathMap.Contains(instanceName))
			{
				UWitnessPath* ts = NewObject<UWitnessPath>();
				ts->InstanceName = instanceName;
				PathMap.Add(instanceName,ts);
			}
			PathMap[instanceName]->Init(node);
		}
		return true;
	}
};
