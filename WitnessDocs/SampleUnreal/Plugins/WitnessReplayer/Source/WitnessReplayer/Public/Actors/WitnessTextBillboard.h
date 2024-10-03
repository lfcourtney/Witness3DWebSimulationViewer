// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WitnessActor.h"
#include "Components/TextRenderComponent.h"
#include "Engine/TextRenderActor.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "WitnessTextBillboard.generated.h"

UCLASS()
class WITNESSREPLAYER_API AWitnessTextBillboard : public AWitnessActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWitnessTextBillboard();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Actor to spawn
	// UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	// TSubclassOf<AActor> actorToSpawn;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	APlayerCameraManager* playerCameraManager;

	// UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	// ATextRenderActor* textRender; 
	
	//UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FString TextValue;
	
};
