#pragma once
#include "WitnessEnviromentObject.h"
#include "XmlFile.h"
#include "WitnessSurface.generated.h"

UCLASS(Blueprintable)
class WITNESSMANAGERS_API UWitnessSurface: public UWitnessEnviromentObject
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadWrite)
	int64 Time;
	UPROPERTY(BlueprintReadWrite)
	FVector BottomLeftCorner;
	UPROPERTY(BlueprintReadWrite)
	FVector TopRightCorner;
	UPROPERTY(BlueprintReadWrite)
	FVector Normal;
	UPROPERTY(BlueprintReadWrite)
	FColor Color1;
	UPROPERTY(BlueprintReadWrite)
	FColor Color2;
	UPROPERTY(BlueprintReadWrite)
	bool bIsSingleSided;
	
	UWitnessSurface(){}

	virtual void Init(const FXmlNode* Node) override
	{
		Time = FCString::Atof(*Node->GetAttribute("time")) * 100000.0f;
		InstanceName = Node->GetAttribute("instanceName");
		
		TArray<FXmlNode*> ChildNodes = Node->GetChildrenNodes();
		for (const FXmlNode* ChildNode : ChildNodes)
		{
			FString Tag = ChildNode->GetTag();
			if(Tag == "surface")
			{
				UE_LOG(LogTemp, Error, TEXT("Parsing Surface"));
				BottomLeftCorner = FVector(
				 FCString::Atof(*ChildNode->GetAttribute("x1")),
				 FCString::Atof(*ChildNode->GetAttribute("y1")),
				 FCString::Atof(*ChildNode->GetAttribute("z1"))
				);
				TopRightCorner = FVector(
				 FCString::Atof(*ChildNode->GetAttribute("x2")),
				 FCString::Atof(*ChildNode->GetAttribute("y2")),
				 FCString::Atof(*ChildNode->GetAttribute("z2"))
				);
				Normal = FVector(
				 FCString::Atof(*ChildNode->GetAttribute("normalX")),
				 FCString::Atof(*ChildNode->GetAttribute("normalY")),
				 FCString::Atof(*ChildNode->GetAttribute("normalZ"))
				);
				Color1 = FColor::FromHex(ChildNode->GetAttribute("color1"));
				Color2 = FColor::FromHex(ChildNode->GetAttribute("color2"));
				bIsSingleSided = (ChildNode->GetAttribute("singleSided") == "True") ? true : false;
				UE_LOG(LogTemp, Error, TEXT("Surface BL: %s"), *BottomLeftCorner.ToString());
			}
		}
	}
};