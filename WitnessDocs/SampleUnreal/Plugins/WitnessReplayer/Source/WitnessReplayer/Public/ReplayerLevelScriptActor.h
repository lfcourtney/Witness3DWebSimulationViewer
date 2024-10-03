// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/IntroWidget.h"
#include "WitnessReplayerActor.h"
#include "Engine/LevelScriptActor.h"
#include "Widgets/VideoControlWidget.h"
#include "ReplayerLevelScriptActor.generated.h"

UCLASS()
class WITNESSREPLAYER_API AReplayerLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AReplayerLevelScriptActor();
	
	UPROPERTY(EditAnywhere)
	EMouseLockMode InitialMouseLockMode;

	UPROPERTY(VisibleAnywhere, Category="Intro Widget")
	UIntroWidget* IntroWidget;
	
	UPROPERTY(EditAnywhere, Category="Intro Widget")
	TSubclassOf<UIntroWidget> IntroWidgetClass;

	UPROPERTY(EditAnywhere, Category="Intro Widget")
	TSubclassOf<UUserWidget> RecordPanelClass;

	UPROPERTY(VisibleAnywhere, Category="Witness Replayer")
	AWitnessReplayerActor* WitnessReplayerActor;
	
	UPROPERTY(EditAnywhere, Category="Witness Replayer")
	UClass* RobotClass;
	
	UPROPERTY(EditAnywhere, Category="Witness Replayer")
	UClass* HumanClass;

	UPROPERTY(EditAnywhere, Category="Witness Replayer")
	UClass* BillboardClass;

	UPROPERTY(EditAnywhere, Category="Xml Loading")
	bool bOpenFileDialog;

	UPROPERTY(EditAnywhere, Category="Xml Loading")
	FString DefaultXmlPath;

	UPROPERTY(VisibleAnywhere, Category="Video Control Widget")
	UVideoControlWidget* VideoControlWidget;
	
	UPROPERTY(EditAnywhere, Category="Video Control Widget")
	TSubclassOf<UVideoControlWidget> VideoControlWidgetClass;

    UPROPERTY(EditAnywhere, Category="Video Control Widget")
	float CameraScale;

	UPROPERTY(EditAnywhere, Category="Video Control Widget")
	float CameraHeight;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	virtual void InitializeXml();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FString GetPathFromCommandLinePath();

	TMap<FString,FString> GetConfigFile();
	FString OpenFileDialogForEditor();
};
