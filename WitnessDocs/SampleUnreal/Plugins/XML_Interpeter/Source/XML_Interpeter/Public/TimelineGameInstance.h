#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WitnessObject.h"
#include "FNavigationTask.h"
#include "Camera/CameraActor.h"
#include "TimelineGameInstance.generated.h"

UCLASS()
class XML_INTERPETER_API UTimelineGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	
	UTimelineGameInstance();
	virtual ~UTimelineGameInstance() override;

	UPROPERTY()
	TMap<FString, AWitnessObject*> TimeLineObjectsMap = TMap<FString, AWitnessObject*>();

	UPROPERTY()
	TArray<FNavigationTask> NavigationTasks = TArray<FNavigationTask>(); // start run -- end run

	bool ContainsName(FString ObjectName)
	{
		return TimeLineObjectsMap.Contains(ObjectName);
	}

	TArray<FString> GetKeys()
	{
		TArray<FString> Keys;
		TimeLineObjectsMap.GetKeys(Keys);
		return Keys;
	}

	AWitnessObject* GetWitnessObjectFromName(FString ObjectName)
	{
		if(TimeLineObjectsMap.Contains(ObjectName))
		{
			AWitnessObject* WitnessObject= *TimeLineObjectsMap.Find(ObjectName);
			if(WitnessObject != nullptr)
			{
				return WitnessObject;
			}
		}
		return nullptr;
	}

	void SimulateNavigationTasks()
	{
		float deltatime = GetWorld()->DeltaTimeSeconds;
		int Length = NavigationTasks.Num();
		for(int i = 0; i < Length; i++)
		{
			FNavigationTask CurrentTask = NavigationTasks[i];
			CurrentTask.NavigationProcess = CurrentTask.NavigationProcess + ((CurrentTask.NavigationSpeed * deltatime) / 10);
			FVector CaculatedPosition = FMath::LerpStable(CurrentTask.Start, CurrentTask.End, CurrentTask.NavigationProcess);
			CurrentTask.Object->SetActorLocation(CaculatedPosition);

			if(CurrentTask.NavigationProcess >= 1)
			{
				NavigationTasks.RemoveAt(i);
				break;
			}

			NavigationTasks[i] = CurrentTask;
		}
	}
	
	
	
	
};
