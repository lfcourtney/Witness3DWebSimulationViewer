// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DatasmithSceneActor.h"
#include "CSVDataStore.generated.h"


USTRUCT(BlueprintType)
struct FAttributeNameValuePair
{
	GENERATED_BODY()
        
	UPROPERTY(BlueprintReadWrite)
	FString Name;

	UPROPERTY(BlueprintReadWrite)
	FString Value;
};

/**
 * 
 */
UCLASS(BlueprintType)
class CSVUTILITIES_API UCSVDataStore : public UObject
{

	GENERATED_BODY()
public:
	UCSVDataStore();
	~UCSVDataStore();

	UFUNCTION(BlueprintCallable)
    void Empty();
	UFUNCTION(BlueprintCallable)
	bool Parse(TArray<FString> lines);
	UFUNCTION(BlueprintCallable)
	bool ParseKVP(TArray<FString> lines);
	UFUNCTION(BlueprintCallable)
	bool HasAttributes(FString key);
	UFUNCTION(BlueprintCallable)
	bool HasAttribute(FString key, FString attribute);
	UFUNCTION(BlueprintCallable)
	TArray<FAttributeNameValuePair> GetAttributes(FString key);
	UFUNCTION(BlueprintCallable)
	FString GetAttribute(FString key, FString attribute);
	UFUNCTION(BlueprintCallable)
	FString GetFormatted(FString key);
	UFUNCTION(BlueprintCallable)
	FString GetHeadersFormatted(FString key);
	UFUNCTION(BlueprintCallable)
	FString GetValuesFormatted(FString key);
	UFUNCTION(BlueprintCallable)
	bool AddToDataStore(FString ObjectId, TMap<FString, FString> Metadata);
	UFUNCTION(BlueprintCallable)
	bool AddToDataStoreByKeys(FString ObjectId, TMap<FString, FString> Metadata, TArray<FString> Keys);
	UFUNCTION(BlueprintCallable)
	bool AddDataSmithToDataStore(TSoftObjectPtr<ADatasmithSceneActor> DatasmithScene, TArray<FString> Keys);

private:
	TMap<FString,TArray<FAttributeNameValuePair>> AttributeMap;
	int LongestAttributeName;
	FString KeyName;
};
