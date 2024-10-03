#pragma once
#include "CoreMinimal.h"
#include "WitnessObject.h"
#include "FNavigationTask.generated.h"

USTRUCT(BlueprintType)
struct FNavigationTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	AWitnessObject* Object;

	UPROPERTY(BlueprintReadWrite)
	float NavigationSpeed;

	UPROPERTY(BlueprintReadWrite)
	FVector Start;

	UPROPERTY(BlueprintReadWrite)
	FVector End;

	UPROPERTY(BlueprintReadWrite)
	FRotator StartRot;

	UPROPERTY(BlueprintReadWrite)
	FRotator EndRot;

	UPROPERTY(BlueprintReadWrite)
	float NavigationProcess;

	UPROPERTY(BlueprintReadWrite)
	int ID;

	FNavigationTask();
	~FNavigationTask();
	FNavigationTask(AWitnessObject* WitnessObject, float Speed, FVector StartPos, FVector EndPos,
		FRotator StartRotation, FRotator EndRotation)
	{
		Object = WitnessObject;
		NavigationSpeed = Speed;
		Start = StartPos;
		End = EndPos;
		StartRot = StartRotation;
		EndRot = EndRotation;
		NavigationProcess = 0;
		ID = Object->GetUniqueID();
	}
};
