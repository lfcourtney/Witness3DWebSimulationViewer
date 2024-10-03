#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actors/WitnessActorSpawner.h"
#include "Actors/WitnessAnimationActor.h"
#include "WitnessManagers/Public/WitnessPathManager/WitnessPathManager.h"
#include "WitnessManagers/Public/WitnessEntityManager/WitnessEntityManager.h"
#include "Actors/WitnessPathActor.h"
#include "Actors/WitnessTextBillboard.h"
#include "WitnessAnimationManager/WitnessAnimationManager.h"
#include "WitnessStaticManager/WitnessStaticManager.h"
#include "WitnessEnviromentManager/WitnessEnviromentManager.h"
#include "WitnessTextManager/WitnessTextManager.h"
#include "WitnessReplayerActor.generated.h"

UCLASS()
class WITNESSREPLAYER_API AWitnessReplayerActor : public AActor
{
	GENERATED_BODY()

public:

	AWitnessReplayerActor();

	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	AWitnessActorSpawner* WitnessActorSpawner;

	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	TMap<FString, AWitnessActor*> ActorMap;

	FXmlFile* XmlFile;

	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	bool bIsPaused = false;

	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	int64 Time = 0;

	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	int64 Duration;

	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	int64 NextTime;

	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	int64 PlaybackSpeed = 25000;

	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	int64 DefaultPlaybackSpeed = 25000;

	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	float PlaybackMultiplier = 1;

	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	int64 PlayBackSpeedPause;
	
	//Managers
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	AWitnessPathManager* WitnessPathManager;

	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	AWitnessEntityManager* WitnessEntityManager;

	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	AWitnessAnimationManager* WitnessAnimationManager;
	
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	AWitnessStaticManager* WitnessStaticManager;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	AWitnessTextManager* WitnessTextManager;

	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	TMap<FString, AWitnessTextBillboard*> BillboardMap;

	
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	TMap<FString, AWitnessAnimationActor*> AnimationMap;

	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	TMap<FString, AWitnessAnimationActor*> LaborMap;

	
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	UClass *mAnimationActorClass = nullptr;
	
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	UClass *mBlueprintHumanClass = nullptr;

	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	UClass *mBlueprintBillboardClass = nullptr;

	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	AWitnessEnviromentManager* WitnessEnviromentManager;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool LoadedFile = false;

protected:
	
	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void GetBlueprintClasses(UClass* RobotClass, UClass* HumanClass, UClass* BillboardClass);

	UFUNCTION(BlueprintCallable)
	bool LoadXML(FString Path);

	UFUNCTION(BlueprintCallable)
	void StepTo(int64 _Time);

	UFUNCTION(BlueprintCallable)
	FString GeometryPathToName(FString GeometryPath);

	//Gets the static mesh path of a witness static mesh object
	UFUNCTION(BlueprintCallable)
	FString GetStaticMeshPath(FString MeshName);

	//Generate Cameras for this world
	UFUNCTION(BlueprintCallable)
	void GenerateCameras(float Scale, float Height, AActor* WorldContext);

	UFUNCTION(BlueprintCallable)
	bool GetFileFromOpenDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes, TArray<FString>& OutFileNames);

	UFUNCTION(BlueprintCallable)
	void SetPaused(bool bShouldPause);

private:
	void CheckTags(FString Path);
};
