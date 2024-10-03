#pragma once
#include "XmlFile.h"
#include "WitnessStatic.generated.h"
UCLASS(Blueprintable)
class WITNESSMANAGERS_API UWitnessStatic: public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	FString InstanceName;
	UPROPERTY(BlueprintReadWrite)
	int64 Time;
	UPROPERTY(BlueprintReadWrite)
	FString Geometry;
	UPROPERTY(BlueprintReadWrite)
	FString QueueParent;
	UPROPERTY(BlueprintReadWrite)
	FString QueuePartPositioning;
	UPROPERTY(BlueprintReadWrite)
	FVector QueuePartRollPitchYaw;
	UPROPERTY(BlueprintReadWrite)
	FVector QueuePartPosition;
	UPROPERTY(BlueprintReadWrite)
	FVector QueuePartDirection;
	UPROPERTY(BlueprintReadWrite)
	FVector Translate;
	UPROPERTY(BlueprintReadWrite)
	FVector Scale;
	UWitnessStatic(){}
	
	void Init(const FXmlNode* Node)
	{
		Time = FCString::Atof(*Node->GetAttribute("time")) * 100000.0f;

		FString tag = Node->GetTag();
		if(tag == "create")
		{
			Geometry = Node->GetAttribute("geometry");
			InstanceName = Node->GetAttribute("instanceName");
			TArray<FXmlNode*> ChildNodes = Node->GetChildrenNodes();
			for (const FXmlNode* ChildNode : ChildNodes)
			{
				FString Tag = ChildNode->GetTag();
				if(Tag == "queueInfo")
				{
					QueueParent = ChildNode->GetAttribute("instanceName");
					TArray<FXmlNode*> GrandChildNodes = ChildNode->GetChildrenNodes();
					for (const FXmlNode* GrandChildNode : GrandChildNodes)
					{
						FString grandChildTag = GrandChildNode->GetTag();
						if(grandChildTag== "behaviour")
						{
							QueuePartPositioning = GrandChildNode->GetAttribute("partPositioning");
							QueuePartRollPitchYaw = FVector(
								 FCString::Atof(*GrandChildNode->GetAttribute("partRoll")),
								 FCString::Atof(*GrandChildNode->GetAttribute("partPitch")),
								 FCString::Atof(*GrandChildNode->GetAttribute("partYaw")));
						}
						if(grandChildTag== "position")
						{
							QueuePartPosition = FVector(
									FCString::Atof(*GrandChildNode->GetAttribute("x")),
									FCString::Atof(*GrandChildNode->GetAttribute("y")),
									FCString::Atof(*GrandChildNode->GetAttribute("z")));
						}
						if(grandChildTag== "direction")
						{
							QueuePartDirection = FVector(
									FCString::Atof(*GrandChildNode->GetAttribute("dx")),
									FCString::Atof(*GrandChildNode->GetAttribute("dy")),
									FCString::Atof(*GrandChildNode->GetAttribute("dz")));
						}
					}
				}
			}
		}
		if(tag == "update")
		{
			TArray<FXmlNode*> ChildNodes = Node->GetChildrenNodes();
			for (const FXmlNode* ChildNode : ChildNodes)
			{
				FString Tag = ChildNode->GetTag();
				if(Tag == "translate")
				{
					Translate = FVector(
						 FCString::Atof(*ChildNode->GetAttribute("x")),
						 FCString::Atof(*ChildNode->GetAttribute("y")),
						 FCString::Atof(*ChildNode->GetAttribute("z")));
				}
				if(Tag == "scale")
				{
					Scale = FVector(
						 FCString::Atof(*ChildNode->GetAttribute("x")),
						 FCString::Atof(*ChildNode->GetAttribute("y")),
						 FCString::Atof(*ChildNode->GetAttribute("z")));
				} else
				{
					Scale = FVector(1,1,1);
				}
				
			}
		}
	}
	
};
