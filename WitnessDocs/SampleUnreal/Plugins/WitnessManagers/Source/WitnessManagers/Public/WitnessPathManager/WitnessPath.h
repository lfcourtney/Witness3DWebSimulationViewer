#pragma once
#include "WitnessPathSegment.h"
#include "Runtime/XmlParser/Public/XmlParser.h"
#include "WitnessPath.generated.h"
UCLASS(Blueprintable)
class WITNESSMANAGERS_API UWitnessPath : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	TArray<UWitnessPathSegment*> Segments = TArray<UWitnessPathSegment*> ();
	UPROPERTY(BlueprintReadWrite)
	float Length;
	UPROPERTY(BlueprintReadWrite)
	int64 Time;
	UPROPERTY(BlueprintReadWrite)
	FString Geometry;
	UPROPERTY(BlueprintReadWrite)
	FString InstanceName;
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
	FVector PathStart;
	UPROPERTY(BlueprintReadWrite)
	float PathWidth;

	UWitnessPath(){}
	
	void Init(const FXmlNode* Node)
	{
		Time = FCString::Atof(*Node->GetAttribute("time")) * 100000.0f;
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
			if(Tag == "path")
			{
				PathStart = FVector(
						FCString::Atof(*ChildNode->GetAttribute("startX")),
						FCString::Atof(*ChildNode->GetAttribute("startY")),
						FCString::Atof(*ChildNode->GetAttribute("startZ")));
				PathWidth = FCString::Atof(*ChildNode->GetAttribute("width"));
				TArray<FXmlNode*> GrandChildNodes = ChildNode->GetChildrenNodes();
				for (const FXmlNode* GrandChildNode : GrandChildNodes)
				{
					UWitnessPathSegment* Seg = NewObject<UWitnessPathSegment>();
					Seg->Init(GrandChildNode);
					Length+=Seg->Length;
					Segments.Add(Seg);
				}
			}
		}
	}

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetPointsAlongPath(const float SPACING) const
	{
		TArray<FVector> points = TArray<FVector>();
		const int num = (Length / SPACING) + 1;
		for(int i = 0; i <= num; i++)
		{
			points.Add(GetPointAt(static_cast<float>(i) / num));
		}
		return points;
	}

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetTangentsAlongPath(const float SPACING) const
	{
		TArray<FVector> tangents = TArray<FVector>();
		const int num = (Length / SPACING) + 1;
		for(int i = 0; i <= num; i++)
		{
			tangents.Add(GetTangentAt(static_cast<float>(i) / num));
		}
		return tangents;
	}

	UFUNCTION(BlueprintCallable)
	FVector GetPointAtExact(const float AT) const
	{
		float LengthAt = AT * Length; 
		for(int i = 0; i < Segments.Num(); i++)
		{
			if(LengthAt >= Segments[i]->Length) LengthAt-= Segments[i]->Length;
			else return Segments[i]->GetPointAt(LengthAt);
		}
		return Segments[Segments.Num()-1]->GetPointAt(LengthAt + Segments[Segments.Num()-1]->Length);
	}

	UFUNCTION(BlueprintCallable)
	FVector GetPointAt(const float AT) const
	{
		return (GetPointAtExact(AT+0.25f/Length) + GetPointAtExact(AT-0.25f/Length)) * 0.5f;
	}

	UFUNCTION(BlueprintCallable)
	FVector GetTangentAt(const float AT) const
	{
		FVector Tangent = GetPointAt(AT+0.25f/Length) - GetPointAt(AT-0.25f/Length);
		Tangent.Normalize();
		return Tangent;
	}

	UFUNCTION(BlueprintCallable)
	FVector GetTangentAtExact(const float AT) const
	{
		FVector Tangent = GetPointAtExact(AT+0.25f/Length) - GetPointAtExact(AT-0.25f/Length);
		Tangent.Normalize();
		return Tangent;
	}
	
};
