#pragma once
#include "WitnessEnviromentObject.h"
#include "XmlFile.h"
#include "WitnessExtrude.generated.h"

UCLASS(Blueprintable)
class WITNESSMANAGERS_API UWitnessExtrude: public UWitnessEnviromentObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	int64 Time;
	UPROPERTY(BlueprintReadWrite)
	FString Texture;
	UPROPERTY(BlueprintReadWrite)
	float Width;
	UPROPERTY(BlueprintReadWrite)
	float Height;
	UPROPERTY(BlueprintReadWrite)
	FVector StartLocation;
	UPROPERTY(BlueprintReadWrite)
	FVector EndLocation;
	
	UWitnessExtrude(){}
	
	virtual void Init(const FXmlNode* Node) override
	{
		Time = FCString::Atof(*Node->GetAttribute("time")) * 100000.0f;
		InstanceName = Node->GetAttribute("instanceName");
		
		TArray<FXmlNode*> ChildNodes = Node->GetChildrenNodes();
		for (const FXmlNode* ChildNode : ChildNodes)
		{
			FString Tag = ChildNode->GetTag();
			if(Tag == "extrude")
			{
				//UE_LOG(LogTemp, Error, TEXT("Parsing Extrude"));
				Texture = Node->GetAttribute("texture");
				Width = FCString::Atof(*Node->GetAttribute("width")) * 100.0f;
				Height = FCString::Atof(*Node->GetAttribute("height")) * 100.0f;
				
				TArray<FXmlNode*> GrandChildNodes = ChildNode->GetChildrenNodes();
				for (const FXmlNode* GrandChildNode : GrandChildNodes)
				{
					FString GrandChildTag = GrandChildNode->GetTag();
					if(GrandChildTag == "line")
					{
						TArray<FXmlNode*> GrandChildChildNodes = ChildNode->GetChildrenNodes();
						for (const FXmlNode* GrandChildChildNode : GrandChildChildNodes)
						{
							StartLocation = FVector(
								 FCString::Atof(*GrandChildChildNode->GetAttribute("startX")),
								 FCString::Atof(*GrandChildChildNode->GetAttribute("startY")),
								 FCString::Atof(*GrandChildChildNode->GetAttribute("startZ"))
							);
							EndLocation = FVector(
								 FCString::Atof(*GrandChildChildNode->GetAttribute("endX")),
								 FCString::Atof(*GrandChildChildNode->GetAttribute("endY")),
								 FCString::Atof(*GrandChildChildNode->GetAttribute("endZ"))
							);
						}
					}
				}
				//UE_LOG(LogTemp, Error, TEXT("Extrude Start: %s"), *StartLocation.ToString());
			}
		}
	}
};
