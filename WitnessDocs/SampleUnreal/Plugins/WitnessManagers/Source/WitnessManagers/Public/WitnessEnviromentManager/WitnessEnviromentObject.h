#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "XmlFile.h"
#include "WitnessEnviromentObject.generated.h"

UCLASS(Blueprintable)
class WITNESSMANAGERS_API UWitnessEnviromentObject : public UObject
{
	GENERATED_BODY()

public:

	UWitnessEnviromentObject(){}
	
	UPROPERTY(BlueprintReadWrite)
	FString InstanceName;

	virtual void Init(const FXmlNode* Node){}
	
};
