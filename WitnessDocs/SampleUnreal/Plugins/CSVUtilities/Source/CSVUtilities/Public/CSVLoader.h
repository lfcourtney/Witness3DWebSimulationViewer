#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <Runtime/Core/Public/Misc/FileHelper.h>
#include "CSVLoader.generated.h"

UCLASS()
class CSVUTILITIES_API UCSVLoader : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "CSV")
	static FString LoadFileToString(FString FileSubPath);

	UFUNCTION(BlueprintCallable, Category = "CSV")
	static TArray<FString> LoadFileToStringArray(FString FileSubPath);
	
};