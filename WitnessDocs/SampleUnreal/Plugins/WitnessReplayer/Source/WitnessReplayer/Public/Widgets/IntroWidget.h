// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/Image.h"
#include "GameFramework/SaveGame.h"

#include "IntroWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartButtonDelegate);

UCLASS()
class WITNESSREPLAYER_API UShowIntroSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	bool bShowIntro;
};

/**
 * 
 */
UCLASS()
class WITNESSREPLAYER_API UIntroWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	UShowIntroSaveGame* ShowIntroSaveGame;

	UPROPERTY(BlueprintReadWrite)
	FString SaveSlotName;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* StartReplayerButton;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* CheckBoxButton;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* CheckImage;

	UPROPERTY(EditAnywhere, BlueprintCallable, BlueprintAssignable)
	FStartButtonDelegate OnStart;

	UFUNCTION()
	void SaveShowIntroSaveGame();

	UFUNCTION()
	void StartReplayer();
	
	virtual void NativeOnInitialized() override;
	
};
