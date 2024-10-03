// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HumanStates.h"
#include "UObject/Object.h"
#include "HumanAnimation.generated.h"

/**
 * 
 */
UCLASS()
class WITNESSREPLAYER_API UHumanAnimation : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "IK")
	float LeftAlpha;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "IK")
	float RightAlpha;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "IK")
	FVector LeftHandle;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "IK")
	FVector RightHandle;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "States")
	TEnumAsByte<EHumanStates> State;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Value")
	float WalkingAt;
};
