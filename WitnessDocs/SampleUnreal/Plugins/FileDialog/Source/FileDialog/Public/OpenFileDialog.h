#pragma once

#include "CoreMinimal.h"
#include "OpenFileDialog.generated.h"

USTRUCT(BlueprintType)
struct FDialogOptions
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FString Title;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FString Path;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FString FilterName;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FString Filter;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FString Extension;
};

UCLASS(Blueprintable)
class FILEDIALOG_API UOpenFileDialog : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	static FString OpenFileDialog(FDialogOptions DialogOptions);
};
