#pragma once

#include "SplineData.h"
#include "WitnessObject.h"
#include "TimelineGameInstance.h"
#include "KismetProceduralMeshLibrary.h"
#include  "FNavigationTask.h"
#include "Action.generated.h"

UCLASS(Blueprintable)
class UAction : public UStruct
{
	GENERATED_BODY()
	
public:
	UAction(){};
	virtual ~UAction(){};

	UPROPERTY()
	UTimelineGameInstance* GameInstance = nullptr;

	UPROPERTY(BlueprintReadWrite)
	int32 Time = 0;

	UFUNCTION(BlueprintCallable)
	virtual void Execute() { UE_LOG(LogTemp, Display, TEXT("Base Action. Time: %d"), Time); }
	
};

UENUM(BlueprintType)
enum class ObjectType : uint8 {
	Machine = 0 UMETA(DisplayName = "Machine"),
	Path = 1 UMETA(DisplayName = "Path"),
	Extrude = 2 UMETA(DisplayName = "Extrude"),
	Surface = 3 UMETA(DisplayName = "Surface")
};

UCLASS(Blueprintable)
class UQueueInfo : public UCreateActionData
{
	GENERATED_BODY()
	
public:
	
	UQueueInfo(){};
	virtual ~UQueueInfo(){};

	UPROPERTY(BlueprintReadWrite)
	EPartPositioning PartPositioning;
	
	UPROPERTY(BlueprintReadWrite)
	FRotator PartRotation;
	
	UPROPERTY(BlueprintReadWrite)
	FVector Position;
	
	UPROPERTY(BlueprintReadWrite)
	FVector Direction;
	
	UPROPERTY(BlueprintReadWrite)
	FString QueueParent;//Queue parent??? a pointer of something
};

UCLASS(Blueprintable)
class UCreateAction : public UAction
{
	GENERATED_BODY()
	
public:
	UCreateAction(){};
	virtual ~UCreateAction(){};
	
	virtual void Execute();

	UPROPERTY(BlueprintReadWrite)
	AWitnessObject* WitnessObject; // to create from values

	UPROPERTY(BlueprintReadWrite)
	TArray<UCreateActionData*> CreateData = TArray<UCreateActionData*>();

	//time
	UPROPERTY(BlueprintReadWrite)
	FString Geometry; //TODO Define Geometry
	UPROPERTY(BlueprintReadWrite)
	FString InstanceName;

	UPROPERTY(BlueprintReadWrite)
	ObjectType ObjectType;
	
	UPROPERTY(BlueprintReadWrite)
	FTransform Transform;
	
};

UCLASS(Blueprintable)
class UUpdateActionData : public UStruct
{
	GENERATED_BODY()
	
public:
	UUpdateActionData(){};
	virtual ~UUpdateActionData(){};
	
};

UCLASS(Blueprintable)
class UTranslate  : public UUpdateActionData
{
	GENERATED_BODY()
	
public:
	UTranslate(){};
	virtual ~UTranslate(){};
	
	UPROPERTY(BlueprintReadWrite)
	FVector Translation;
	
};

UCLASS(Blueprintable)
class UScale  : public UUpdateActionData
{
	GENERATED_BODY()
	
public:
	UScale(){};
	virtual ~UScale(){};
	
	UPROPERTY(BlueprintReadWrite)
	FVector ScaleChange;
	
};

UCLASS(Blueprintable)
class URotate  : public UUpdateActionData
{
	GENERATED_BODY()
	
public:
	URotate(){};
	virtual ~URotate(){};
	
	UPROPERTY(BlueprintReadWrite)
	FVector RotationChange;
	
};

UCLASS(Blueprintable)
class UPartPosition  : public UUpdateActionData
{
	GENERATED_BODY()
	
public:
	UPartPosition(){};
	virtual ~UPartPosition(){};
	
	UPROPERTY(BlueprintReadWrite)
	FString InstanceName; //Current attachment or the object we are linked/parented to

	UPROPERTY(BlueprintReadWrite)
	float Position; //Percentage from 0.0 to 1 how far into the path

	UPROPERTY(BlueprintReadWrite)
	bool bIsReverse = false; // go forward or backwards should default to false or true not yet know //TODO should reverse be standard set to true or false
	
};

UCLASS(Blueprintable)
class UUpdateAction : public UAction
{
	GENERATED_BODY()
	
public:
	UUpdateAction(){};
	virtual ~UUpdateAction(){};
	
	virtual void Execute() override;
	virtual void Execute(AWitnessObject* Object);

	UPROPERTY(BlueprintReadWrite)
	AWitnessObject* WitnessObject; // to get from the instance name

	UPROPERTY(BlueprintReadWrite)
	TArray<UUpdateActionData*> UpdateData; // go through all changes

	UPROPERTY(BlueprintReadWrite)
	FString InstanceName; // object that is doing the update
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsVisible; // check if changed
	
};

UCLASS(Blueprintable)
class UDeleteAction : public UAction
{
	GENERATED_BODY()
	
public:
	UDeleteAction(){};
	virtual ~UDeleteAction(){};
	
	virtual void Execute() override;

	UPROPERTY(BlueprintReadWrite)
	FString InstanceName;
};

UCLASS(Blueprintable)
class ULoadAction : public UAction
{
	GENERATED_BODY()
	
public:
	ULoadAction(){};
	virtual ~ULoadAction(){};
	
	virtual void Execute() override;

	UPROPERTY(BlueprintReadWrite)
	FString ModelName;
};

UENUM(BlueprintType)
enum class EStartType : uint8 {
	Session = 0 UMETA(DisplayName = "Session"),
	Build = 1 UMETA(DisplayName = "Build"),
	Run = 2 UMETA(DisplayName = "Run"),
};

UCLASS(Blueprintable)
class UStartAction : public UAction
{
	GENERATED_BODY()
	
public:
	UStartAction(){};
	virtual ~UStartAction(){};
	
	virtual void Execute() override;

	UPROPERTY(BlueprintReadWrite)
	EStartType Type;
};

UCLASS(Blueprintable)
class UEndAction : public UAction
{
	GENERATED_BODY()
	
public:
	UEndAction(){};
	virtual ~UEndAction(){};
	
	virtual void Execute() override;

	UPROPERTY(BlueprintReadWrite)
	EStartType Type;
};

