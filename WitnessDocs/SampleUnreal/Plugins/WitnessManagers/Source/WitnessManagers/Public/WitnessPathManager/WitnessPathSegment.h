#include "Runtime/XmlParser/Public/XmlParser.h"
#include "WitnessPathSegment.generated.h"
#pragma once
UCLASS(Blueprintable)
class WITNESSMANAGERS_API UWitnessPathSegment: public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	float Length;
	UPROPERTY(BlueprintReadWrite)
	float Angle;
	UPROPERTY(BlueprintReadWrite)
	float StartRadius;
	UPROPERTY(BlueprintReadWrite)
	float EndRadius;
	UPROPERTY(BlueprintReadWrite)
	float StartAngle;
	UPROPERTY(BlueprintReadWrite)
	float EndAngle;
	
	UPROPERTY(BlueprintReadWrite)
	bool IsArc;
	
	UPROPERTY(BlueprintReadWrite)
	FVector Start;
	UPROPERTY(BlueprintReadWrite)
	FVector End;
	UPROPERTY(BlueprintReadWrite)
	FVector Centre;
	
	UPROPERTY(BlueprintReadWrite)
	FString SweepDirection;

	UWitnessPathSegment(){}

	void Init(const FXmlNode* Node)
	{
		const FString tag = Node->GetTag();

		if(tag == "line")
		{
			IsArc = false;
			Start = FVector(
				FCString::Atof(*Node->GetAttribute("startX")),
				FCString::Atof(*Node->GetAttribute("startY")),
				FCString::Atof(*Node->GetAttribute("startZ")));
			End = FVector(
				FCString::Atof(*Node->GetAttribute("endX")),
				FCString::Atof(*Node->GetAttribute("endY")),
				FCString::Atof(*Node->GetAttribute("endZ")));
			Length = (Start - End).Length();
		}
		if(tag == "arc")
		{
			IsArc = true;
			Start = FVector(
				FCString::Atof(*Node->GetAttribute("startX")),
				FCString::Atof(*Node->GetAttribute("startY")),
				FCString::Atof(*Node->GetAttribute("startZ")));
			End = FVector(
				FCString::Atof(*Node->GetAttribute("endX")),
				FCString::Atof(*Node->GetAttribute("endY")),
				FCString::Atof(*Node->GetAttribute("endZ")));
			Centre = FVector(
				FCString::Atof(*Node->GetAttribute("centreX")),
				FCString::Atof(*Node->GetAttribute("centreY")),
				FCString::Atof(*Node->GetAttribute("centreZ")));
			Angle = FCString::Atof(*Node->GetAttribute("Angle"));

			SweepDirection = *Node->GetAttribute("sweepDirection");

			const FVector2f Start2D = FVector2f(Start.X,Start.Z);
			const FVector2f End2D = FVector2f(End.X,End.Z);
			const FVector2f Centre2D = FVector2f(Centre.X,Centre.Z);
			
			StartRadius = (Start2D-Centre2D).Length();
			EndRadius = (End2D-Centre2D).Length();
			Length = (StartRadius+EndRadius) * PI * Angle / 360.0f;

			FVector2f Start2DNorm = Start2D - Centre2D;
			Start2DNorm.Normalize();
			FVector2f End2DNorm = End2D - Centre2D;
			End2DNorm.Normalize();
			StartAngle = atan2f(Start2DNorm.Y, Start2DNorm.X) / PI * 180.0f;
			EndAngle = atan2f(End2DNorm.Y, End2DNorm.X) / PI * 180.0f;
			if(EndAngle > StartAngle && SweepDirection != "clockwise") EndAngle-=360;
			if(EndAngle < StartAngle && SweepDirection == "clockwise") EndAngle+=360;
		}
		
	}

	FVector GetPointAt(const float AT) const
	{
		const float LerpFac = AT/Length;
		if(IsArc)
		{
			const float A = StartAngle * (1.0f-LerpFac) + EndAngle * LerpFac;
			const float H = Start.Y * (1.0f-LerpFac) + End.Y * LerpFac - Centre.Y;
			const float R = StartRadius * (1.0f-LerpFac) + EndRadius * LerpFac;
			return Centre + FVector(cosf(A * PI/ 180.0f) * R, H, sinf(A * PI/ 180.0f) * R);
		}
		return Start * (1.0f-LerpFac) + End * LerpFac;
	}
};
