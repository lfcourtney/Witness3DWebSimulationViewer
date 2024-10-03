// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Runtime/XmlParser/Public/XmlParser.h"
#include "WitnessEntity.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class WITNESSMANAGERS_API UWitnessEntity : public UObject
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadWrite)
	int64 Time;
	UPROPERTY(BlueprintReadWrite)
	bool Visible;
	UPROPERTY(BlueprintReadWrite)
	FString InstanceName;
	UPROPERTY(BlueprintReadWrite)
	FString QueueParent;
	UPROPERTY(BlueprintReadWrite)
	FString PartPositioning;
	UPROPERTY(BlueprintReadWrite)
	FString Geometry;
	UPROPERTY(BlueprintReadWrite)
	float PartRoll;
	UPROPERTY(BlueprintReadWrite)
	float PartPitch;
	UPROPERTY(BlueprintReadWrite)
	float PartYaw;
	UPROPERTY(BlueprintReadWrite)
	float Px;
	UPROPERTY(BlueprintReadWrite)
	float Py;
	UPROPERTY(BlueprintReadWrite)
	float Pz;
	UPROPERTY(BlueprintReadWrite)
	float Dx;
	UPROPERTY(BlueprintReadWrite)
	float Dy;
	UPROPERTY(BlueprintReadWrite)
	float Dz;
	UPROPERTY(BlueprintReadWrite)
	float Sx;
	UPROPERTY(BlueprintReadWrite)
	float Sy;
	UPROPERTY(BlueprintReadWrite)
	float Sz;
	UPROPERTY(BlueprintReadWrite)
	FString PartPositionInstanceName;
	UPROPERTY(BlueprintReadWrite)
	float PartPosition;
	UPROPERTY(BlueprintReadWrite)
	bool PartPositionReversed;
	UPROPERTY(BlueprintReadWrite)
	bool Deleted;
	
	UWitnessEntity(){};
	void InitZero()
	{
		Time = 0;
		Visible =false;
		InstanceName = "";
		QueueParent = "";
		PartPositioning = "";
		Geometry = "";
		PartRoll = PartPitch = PartYaw = Px = Py = Pz = Dx = Dy = Dz = Sx = Sy = Sz = 0;
		PartPositionInstanceName = "";
		PartPosition = 0;
		PartPositionReversed = false;
		Deleted = true;
	};
	//
	// UWitnessEntity(UWitnessEntity prev)
	// {
	// 	init(prev);
	// }


	void InitPrev(UWitnessEntity* prev)
	{
		Time = prev->Time;//
		Visible = prev->Visible;//
		InstanceName = prev->InstanceName;//
		QueueParent = prev->QueueParent;//
		PartPositioning = prev->PartPositioning;//
		Geometry = prev->Geometry;//
		PartRoll = prev->PartRoll;//
		PartPitch = prev->PartPitch;//
		PartYaw = prev->PartYaw;//
		Px = prev->Px;//
		Py = prev->Py;//
		Pz = prev->Pz;//
		Dx = prev->Dx;//
		Dy = prev->Dy;//
		Dz = prev->Dz;//
		Sx = prev->Sx;//
		Sy = prev->Sy;//
		Sz = prev->Sz;//
		PartPositionInstanceName = prev->PartPositionInstanceName;//
		PartPosition = prev->PartPosition;//
		PartPositionReversed = prev->PartPositionReversed;//
		Deleted = prev->Deleted;//
	}
	

	
	void InitPrev(UWitnessEntity* prev, FXmlNode* Node)
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
			//Deleted = true;
			Geometry = Node->GetAttribute("geometry");
			for (FXmlNode* childNode : childNodes)
			{
				FString childTag = childNode->GetTag();
				if(childTag== "queueInfo")
				{
					QueueParent = childNode->GetAttribute("QueueParent");
					auto grandChildNodes = childNode->GetChildrenNodes();
					for (FXmlNode* grandChildNode : grandChildNodes)
					{
						FString grandChildTag = grandChildNode->GetTag();
						if(grandChildTag== "behaviour")
						{
							PartPositioning = grandChildNode->GetAttribute("partPositioning");
							PartRoll = FCString::Atof(*grandChildNode->GetAttribute("partRoll"));
							PartPitch = FCString::Atof(*grandChildNode->GetAttribute("partPitch"));
							PartYaw = FCString::Atof(*grandChildNode->GetAttribute("partYaw"));
						}
						if(grandChildTag== "position")
						{
							Px = FCString::Atof(*grandChildNode->GetAttribute("x"));
							Py = FCString::Atof(*grandChildNode->GetAttribute("y"));
							Pz = FCString::Atof(*grandChildNode->GetAttribute("z"));
						}
						if(grandChildTag== "direction")
						{
							Dx = FCString::Atof(*grandChildNode->GetAttribute("dx"));
							Dy = FCString::Atof(*grandChildNode->GetAttribute("dy"));
							Dz = FCString::Atof(*grandChildNode->GetAttribute("dz"));
						}
					}
				}
			}
		}
		if(tag ==  "update")
		{
			Deleted = false;
			FString visibAtt = Node->GetAttribute("visible");
			if(visibAtt == "true" || visibAtt == "false") Visible = visibAtt == "true";
			for (FXmlNode* childNode : childNodes)
			{
				FString childTag = childNode->GetTag();
				if(childTag== "scale")
				{
					Sx = FCString::Atof(*childNode->GetAttribute("x"));
					Sy = FCString::Atof(*childNode->GetAttribute("y"));
					Sz = FCString::Atof(*childNode->GetAttribute("z"));
				}
				if(childTag==  "partPosition")
				{
					PartPositionInstanceName = childNode->GetAttribute("instanceName");
					PartPosition =  FCString::Atof(*childNode->GetAttribute("position"));
					FString revAtt = Node->GetAttribute("visible");
					if(revAtt == "true" || revAtt == "false") PartPositionReversed = revAtt == "true";
				}
			}
		}
		if(tag ==  "delete")
		{
			Deleted = true;
		}
	}
	//virtual ~UWitnessEntity(){};

	float lerp(float a, float b, float f)
	{
		return a * (1.0-f) + b * f;
	}

	void Interpolate(UWitnessEntity* A, UWitnessEntity* B, int64 timeAt)
	{
		float f = (B->Time==A->Time) ? 0 : ((float)(timeAt-A->Time)) / (B->Time-A->Time);
		InitPrev(A);
		Time = lerp(A->Time, B->Time, f);
		PartRoll = lerp(A->PartRoll, B->PartRoll, f);
		PartPitch = lerp(A->PartPitch, B->PartPitch, f);
		PartYaw = lerp(A->PartYaw, B->PartYaw, f);
		Px = lerp(A->Px, B->Px, f);
		Py = lerp(A->Py, B->Py, f);
		Pz = lerp(A->Pz, B->Pz, f);
		Dx = lerp(A->Dx, B->Dx, f);
		Dy = lerp(A->Dy, B->Dy, f);
		Dz = lerp(A->Dz, B->Dz, f);
		Sx = lerp(A->Sx, B->Sx, f);
		Sy = lerp(A->Sy, B->Sy, f);
		Sz = lerp(A->Sz, B->Sz, f);
		if(A->PartPositionInstanceName == B->PartPositionInstanceName) PartPosition = lerp(A->PartPosition, B->PartPosition, f);
	}
};
