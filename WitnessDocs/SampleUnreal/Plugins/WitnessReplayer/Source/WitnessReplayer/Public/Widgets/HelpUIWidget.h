// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "HelpUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class WITNESSREPLAYER_API UHelpUIWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
		UButton* ContinueButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* ExitButton;
	
};
