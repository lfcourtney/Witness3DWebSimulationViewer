// Fill out your copyright notice in the Description page of Project Settings.


#include "CSVDataStore.h"

#include "DatasmithAssetUserData.h"
#include "DatasmithContentBlueprintLibrary.h"
#include "DatasmithSceneActor.h"
#include "Kismet/KismetSystemLibrary.h"

UCSVDataStore::UCSVDataStore()
{
}

UCSVDataStore::~UCSVDataStore()
{
}

void UCSVDataStore::Empty()
{
	AttributeMap.Empty();
}

bool UCSVDataStore::Parse(TArray<FString> lines)
{

	if(lines.Num()==0) return false;
	FString headerLine = lines[0];
	TArray<FString> headers;
	int attributeNum = headerLine.ParseIntoArray(headers, TEXT(","),false);
	for(int i = 0; i < headers.Num(); i++)
	{
		headers[i] = headers[i].TrimStartAndEnd();
	}
	KeyName = headers[0];
	for(int i = 1; i < lines.Num(); i++)
	{
		FString line = lines[i];
		TArray<FString> attributes;
		line.ParseIntoArray(attributes, TEXT(","),false);
		FString assetId = attributes[0];
		TArray<FAttributeNameValuePair> attributeArray;
		for(int j = 1; j < attributeNum; j++)
		{
			FString att = attributes[j].TrimStartAndEnd();
			FAttributeNameValuePair data;
			data.Name = headers[j];
			data.Value = att;
			if(!att.IsEmpty()) attributeArray.Add(data);
		}
		AttributeMap.Add(assetId,attributeArray);
	}
	LongestAttributeName = 0;
	for(int i = 0; i < headers.Num(); i++)
	{
		if(headers[i].Len() > LongestAttributeName) LongestAttributeName = headers[i].Len();
	}
	return true;
}

bool UCSVDataStore::ParseKVP(TArray<FString> lines)
{
	if(lines.Num()==0) return false;
	for(int i = 1; i < lines.Num(); i++)
	{
		FString line = lines[i];
		TArray<FString> attributes;
		line.ParseIntoArray(attributes, TEXT(","),false);
		FString assetId = attributes[0];
		if(!AttributeMap.Contains(assetId))
		{
			TArray<FAttributeNameValuePair> attributeArray;
			AttributeMap.Add(assetId,attributeArray);
		}
		bool contains  = false;
		for(int j = 0; j < AttributeMap[assetId].Num(); j++)
		{
			if(AttributeMap[assetId][j].Name==attributes[1])
			{
				AttributeMap[assetId][j].Value = attributes[2];
				contains = true;
			}
		}
		if(!contains)
		{
			FAttributeNameValuePair data;
			data.Name = attributes[1];
			data.Value = attributes[2];
			AttributeMap[assetId].Add(data);
		}

	}
	return true;
}

bool UCSVDataStore::HasAttributes(FString key)
{
	return AttributeMap.Contains(key);
}

bool UCSVDataStore::HasAttribute(FString key, FString attribute)
{
	TArray<FAttributeNameValuePair> attributes = GetAttributes(key);
	for(int i = 0; i < attributes.Num(); i++) if(attributes[i].Name == attribute) return true;
	return false;
}

TArray<FAttributeNameValuePair> UCSVDataStore::GetAttributes(FString key)
{
	if(HasAttributes(key))
		return AttributeMap[key];
	return TArray<FAttributeNameValuePair>();
}

FString UCSVDataStore::GetAttribute(FString key, FString attribute)
{
	TArray<FAttributeNameValuePair> attributes = GetAttributes(key);
	for(int i = 0; i < attributes.Num(); i++) if(attributes[i].Name == attribute) return attributes[i].Value;
	return "";
}

FString UCSVDataStore::GetFormatted(FString key)
{
	TArray<FAttributeNameValuePair> attributes = GetAttributes(key);
	FString accum;// = KeyName.RightPad(LongestAttributeName) + " : " + key + "\n";
	for(int i = 0; i < attributes.Num(); i++)
	{
		if(attributes[i].Name == "URL") continue;
		FString attribName = attributes[i].Name;
		accum+= attribName.RightPad(LongestAttributeName)+ " : " + attributes[i].Value + "\n";
	}
	return accum.Left(accum.Len()-1);
}

FString UCSVDataStore::GetHeadersFormatted(FString key)
{
	TArray<FAttributeNameValuePair> attributes = GetAttributes(key);
	FString accum;
	for(int i = 0; i < attributes.Num(); i++)
	{
		if(attributes[i].Name == "URL") continue;
		accum+=attributes[i].Name+ "\n";
	}
	return accum.Left(accum.Len()-1);
}

FString UCSVDataStore::GetValuesFormatted(FString key)
{
	TArray<FAttributeNameValuePair> attributes = GetAttributes(key);
	FString accum;
	for(int i = 0; i < attributes.Num(); i++)
	{
		if(attributes[i].Name == "URL") continue;
		accum+=attributes[i].Value+ "\n";
	}
	return accum.Left(accum.Len()-1);
}

bool UCSVDataStore::AddToDataStore(FString ObjectId, TMap<FString, FString> Metadata)
{
	TArray<FString> keys;
	Metadata.GetKeys(keys);
	return AddToDataStoreByKeys(ObjectId, Metadata,keys );
}

bool UCSVDataStore::AddToDataStoreByKeys(FString ObjectId, TMap<FString, FString> Metadata, TArray<FString> Keys)
{
	if(!AttributeMap.Contains(ObjectId))
	{
		const TArray<FAttributeNameValuePair> attributeArray;
		AttributeMap.Add(ObjectId,attributeArray);
	}
	for(int i = 0; i < Keys.Num(); i++)
	{
		bool has = false;
		if(!Metadata.Contains(Keys[i])) continue;
		for (int j = 0; j < AttributeMap[ObjectId].Num(); j++)
		{
			if(AttributeMap[ObjectId][j].Name == Keys[i])
			{
				has = true;
				AttributeMap[ObjectId][j].Value = Metadata[Keys[i]];
			}
		}
		if(!has)
		{
			FAttributeNameValuePair data;
			data.Name = Keys[i];
			data.Value = Metadata[Keys[i]];
			AttributeMap[ObjectId].Add(data);
		}
	}
	return true;
}

bool UCSVDataStore::AddDataSmithToDataStore(TSoftObjectPtr<ADatasmithSceneActor> DatasmithScene, TArray<FString> Keys)
{
	if(Keys.Num() <= 0)
	{
		return false;
	}
	UE_LOG(LogTemp, Error, TEXT("N > 0"));

	TArray<FName> ActorNames;
	DatasmithScene->RelatedActors.GetKeys(ActorNames);

	for (FName ActorName : ActorNames)
	{

		TSoftObjectPtr<AActor> Actor = *DatasmithScene->RelatedActors.Find(ActorName);
		//UE_LOG(LogTemp, Error, TEXT("%s"),*(Actor->GetName()));
		UDatasmithAssetUserData* AssetUserData = UDatasmithContentBlueprintLibrary::GetDatasmithUserData(Actor.Get());

		if(AssetUserData == nullptr)
		{
			continue;
		}
		UE_LOG(LogTemp, Error, TEXT("AssetUserData != nullptr"));
		FString ActorLabel = Actor->GetName();
		UE_LOG(LogTemp, Error, TEXT("%s"),*ActorLabel);
		for(FString Key : Keys)
		{
			UE_LOG(LogTemp, Error, TEXT("%s"),*Key);
			if(!AttributeMap.Contains(ActorLabel))
			{
				AttributeMap.Add(ActorLabel, TArray<FAttributeNameValuePair>());
			}

			FString DatasmithKey = FString::Printf(TEXT("Element_%s"), *Key);
			FString* Value = AssetUserData->MetaData.Find(FName(DatasmithKey));
			
			if(Value != nullptr)
			{
				AttributeMap[ActorLabel].Add({Key, *Value});
			}
		}
	}
	
	return true;
}
