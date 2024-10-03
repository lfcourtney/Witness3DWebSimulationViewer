#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Runtime/XmlParser/Public/XmlParser.h"
#include "WitnessText.generated.h"

UCLASS(Blueprintable)
class  WITNESSMANAGERS_API UWitnessText : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	int64 Time;
	UPROPERTY(BlueprintReadWrite)
	FString InstanceName;
	UPROPERTY(BlueprintReadWrite)
	FString Text;
	UPROPERTY(BlueprintReadWrite)
	float Px;
	UPROPERTY(BlueprintReadWrite)
	float Py;
	UPROPERTY(BlueprintReadWrite)
	float Pz;
	
	
	UWitnessText(){};
	void InitZero()
	{
		Text = "";
		Time = 0;
		InstanceName = "";
		
	};
	//
	// UWitnessEntity(UWitnessEntity prev)
	// {
	// 	init(prev);
	// }


	void InitPrev(UWitnessText* prev)
	{
		Text = prev->Text;
		Time = prev->Time;//
		InstanceName = prev->InstanceName;//
		Px = prev->Px;//
		Py = prev->Py;//
		Pz = prev->Pz;//
	}
	

	
	void InitPrev(UWitnessText* prev, FXmlNode* Node)
	{
		InitPrev(prev);

		FString Convert = Node->GetAttribute("time");
		Convert = Convert.Replace(TEXT("."),TEXT(""));
		Time = FCString::Atoi64(*Convert);
		InstanceName = Node->GetAttribute("instanceName");
		
		FString tag = Node->GetTag();
		auto childNodes = Node->GetChildrenNodes();
		if(tag == "create")
		{
				Px = FCString::Atof(*Node->GetAttribute("x"));
				Py = FCString::Atof(*Node->GetAttribute("y"));
				Pz = FCString::Atof(*Node->GetAttribute("z"));
			
			for (FXmlNode* childNode : childNodes)
			{
				FString childTag = childNode->GetTag();
				if(childTag== "text")
				{
					Text = childNode->GetContent();
				}
			}
		}
		if(tag ==  "update")
		{

			for (FXmlNode* childNode : childNodes)
			{
				FString childTag = childNode->GetTag();
				if(childTag== "text")
				{
					Text = childNode->GetContent();
				}
			}
		}
	}
	//virtual ~UWitnessEntity(){};

	float lerp(float a, float b, float f)
	{
		return a * (1.0-f) + b * f;
	}

	void Interpolate(UWitnessText* A, UWitnessText* B, int64 timeAt)
	{
		float f = (B->Time==A->Time) ? 0 : ((float)(timeAt-A->Time)) / (B->Time-A->Time);
		InitPrev(A);
		Time = lerp(A->Time, B->Time, f);
		Px = lerp(A->Px, B->Px, f);
		Py = lerp(A->Py, B->Py, f);
		Pz = lerp(A->Pz, B->Pz, f);
	}
	
};
