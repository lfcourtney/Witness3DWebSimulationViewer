#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "SplineData.generated.h"

UCLASS(Blueprintable)
class UPathData : public UStruct
{
	GENERATED_BODY()
	
public:
	UPathData(){};
	virtual ~UPathData(){};
	
};

UCLASS(Blueprintable)
class ULine : public UPathData
{
	GENERATED_BODY()
	
public:
	ULine(){};
	virtual ~ULine(){};

	UPROPERTY(BlueprintReadWrite)
	FVector Start;
	
	UPROPERTY(BlueprintReadWrite)
	FVector End;
	
};

UCLASS(Blueprintable)
class UArc : public UPathData
{
	GENERATED_BODY()
	
public:
	UArc(){};
	virtual ~UArc(){};
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsClockwise; //Should parts move clock or counter-clock wise
	
	UPROPERTY(BlueprintReadWrite)
	float Angle; // angle of the arc
	
	UPROPERTY(BlueprintReadWrite)
	FVector Start;
	
	UPROPERTY(BlueprintReadWrite)
	FVector End;
	
	UPROPERTY(BlueprintReadWrite)
	FVector Centre;
	
};

UENUM(BlueprintType)
enum class EPartPositioning : uint8 {
	PartOver = 0 UMETA(DisplayName = "Over"),
	PartUnder = 1 UMETA(DisplayName = "Under"),
};

UCLASS(Blueprintable)
class UCreateActionData : public UStruct
{
	GENERATED_BODY()
	
public:
	UCreateActionData(){};
	virtual ~UCreateActionData(){};
	
};

UCLASS(Blueprintable)
class UPath : public UCreateActionData
{
	GENERATED_BODY()
	
public:
	UPath(){};
	virtual ~UPath(){};

	UPROPERTY(BlueprintReadWrite)
	TArray<UPathData*> PathData = TArray<UPathData*>();
	
	UPROPERTY(BlueprintReadWrite)
	FVector Startlocation;
	
	UPROPERTY(BlueprintReadWrite)
	float width;
	
};

UCLASS(Blueprintable)
class UExtrude : public UCreateActionData
{
	GENERATED_BODY()
	
public:
	UExtrude(){};
	virtual ~UExtrude(){};

	UPROPERTY(BlueprintReadWrite)
	ULine* Line;//until now always one TODO: Is it always one?
	
	UPROPERTY(BlueprintReadWrite)
	float Width;

	UPROPERTY(BlueprintReadWrite)
	float Height;

	UPROPERTY(BlueprintReadWrite)
	FString Texture;
	
};

UCLASS(Blueprintable)
class USurface : public UCreateActionData
{
	GENERATED_BODY()
	
public:
	USurface(){};
	virtual ~USurface(){};

	UPROPERTY(BlueprintReadWrite)
	FVector StartLoc;
	UPROPERTY(BlueprintReadWrite)
	FVector EndLoc;

	UPROPERTY(BlueprintReadWrite)
	FVector Normal;

	UPROPERTY(BlueprintReadWrite)
	FColor color1;
	UPROPERTY(BlueprintReadWrite)
	FColor color2;
	UPROPERTY(BlueprintReadWrite)
	bool bIsSingleSided;
	
};