#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Action.h"
#include "Algo/ForEach.h"
#include "Runtime/XmlParser/Public/XmlParser.h"
#include "XML_Interpeter.h"
#include "FNavigationTask.h"
#include "Kismet/KismetMathLibrary.h"
#include "XMLInterpeter.generated.h"

UCLASS(Blueprintable)
class UTimeFrame : public UStruct
{
	GENERATED_BODY()
public:

	UTimeFrame() { Time = 0; Actions = TArray<UAction*>(); }
	UTimeFrame(int _Time) { Time = _Time; }
	~UTimeFrame(){}

	UPROPERTY(BlueprintReadWrite)
	int Time = 0;
	UPROPERTY(BlueprintReadWrite)
	TArray<UAction*> Actions = TArray<UAction*>();
	
};

UCLASS(Blueprintable)
class XML_INTERPETER_API AUxmlInterpeter : public AActor
{
public:
	GENERATED_BODY()
	
	AUxmlInterpeter(){};
	~AUxmlInterpeter(){};

	UFUNCTION(BlueprintCallable)
	void Init(FString Path);

	UPROPERTY()
	UTimelineGameInstance* GameInstancePtr;
	
	FXmlFile* XmlFile;

	UPROPERTY()
	TArray<UAction*> Actions = TArray<UAction*>();//temp

	UPROPERTY()
	TArray<UAction*> Initialize = TArray<UAction*>();// start build -- end build

	UPROPERTY()
	TArray<UAction*> Update = TArray<UAction*>();// start run -- end run

	UPROPERTY()
	FTimerHandle SimulateTimerHandle;
	//Gets the witness object name from the XML geometry path.
	//From -> C:\Users\Public\Documents\WITNESS 23 Horizon\W3D\Assets\Shapes\dg-vh-Agv1
	//To   -> dg-vh-Agv1
	UFUNCTION(BlueprintCallable)
	FString GeometryPathToName(FString GeometryPath);

	//Gets the static mesh path of a witness static mesh object
	UFUNCTION(BlueprintCallable)
	FString GetStaticMeshPath(FString MeshName);

	UFUNCTION(BlueprintCallable)
	FString GetMeshPathFolder(FString MeshName)
	{
		if(MeshName.Contains(" "))
		{
			MeshName = MeshName.Replace(TEXT(" "),TEXT("_"));
		}
	
		FString ReturnValue = "/Game/Models/" + MeshName + '/';
		return ReturnValue;
	}

	UFUNCTION(BlueprintCallable)
	static UMaterialInstanceDynamic* GetMaterial(const FString TextureFilePath);

	UFUNCTION(BlueprintCallable)
	static UTexture* GetTexture(const FString TextureFilePath);

	UFUNCTION(BlueprintCallable)
	static void GenerateCameras(float Scale,float Height , AActor* WorldContext);
	
	FString ModelName;
	float ModelVersion;
	
	UFUNCTION(BlueprintCallable)
	bool LoadXMLFile(FString path);

	UFUNCTION(BlueprintCallable)
	void CreateTimeStamps(TArray<UAction*> _Actions);

	void GetActions(FXmlNode* CurrentNode);

	void SimulateNavigation() const;

	static float GetRotationDifference(const FRotator A, const FRotator B);

	UFUNCTION(BlueprintCallable)
	int GetIndexFromTime(int64 time);

	UFUNCTION(BlueprintCallable)
	TArray<UAction*>& GetActions() { return Actions; }

	UFUNCTION(BlueprintCallable)
	TArray<UAction*>& GetInitActions() { return Initialize; }

	UFUNCTION(BlueprintCallable)
	TArray<UAction*>& GetUpdateActions() { return Update; }

	//UPROPERTY()
	//TArray<UTimeFrame*> TimeFrames = TArray<UTimeFrame*>();

	UPROPERTY()
	TMap<int, UTimeFrame*> TimeFrames = TMap<int, UTimeFrame*>();
	
	//UFUNCTION(BlueprintPure)
	//TArray<UTimeFrame*>& GetTimeFrames() { return TimeFrames; }

	UFUNCTION(BlueprintPure)
	TMap<int, UTimeFrame*>& GetTimeFrames() { return TimeFrames; }

	//UFUNCTION(BlueprintPure)
	//int GetDurationTime() { return TimeFrames.Last()->Time; }

	UFUNCTION(BlueprintPure)
	int GetDurationTime()
	{
		TArray<UTimeFrame*> Frames;
		TimeFrames.GenerateValueArray(Frames);
		return Frames[Frames.Num()-1]->Time;
	}

	int CurrentTimeIndex = 0; // should be a index of the keys
	int CurrentTime = 0; // should be a index of the keys

	UFUNCTION(BlueprintCallable)
	void StartTimeline();

	UFUNCTION(BlueprintCallable)
	void UpdateTimeline(); // iterate to next timestamp and execute all actions

	UFUNCTION(BlueprintCallable)
	void GoToTime(int Time); // where time is actual data time not a key index
	
	UFUNCTION(BlueprintCallable)
	AActor* SpawnObject(ObjectType ToSpawn, FTransform Transform);
	
	UPROPERTY(BlueprintReadWrite)
	AActor* WorldContext;

	UFUNCTION(BlueprintCallable)
	void SetWorldContext(AActor* _WorldContext){ WorldContext = _WorldContext; }

	
	
};


