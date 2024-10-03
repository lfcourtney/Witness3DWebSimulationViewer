// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HumanAnimation.h"
#include "GameFramework/Actor.h"
#include "Actors/WitnessActor.h"
#include "Actors/WitnessAnimationActor.h"
#include "Human/HumanStates.h"
#include "WitnessHuman.generated.h"

UCLASS(Blueprintable)
class WITNESSREPLAYER_API AWitnessHuman : public AWitnessAnimationActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWitnessHuman();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Components")
	UChildActorComponent* EffectPoint;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Caculating")
	FVector OldPosition;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Animation Values")
	FVector RightHandle;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Animation Values")
	FVector LeftHandle;

private:
	UHumanAnimation* AnimationInstance;

protected:
	void BeginPlay() override;

private:
	void Load();
	
	void Tick(float DeltaSeconds) override;

	void SetEffector();

	void CheckState();

	void CaculateHandles();
};
