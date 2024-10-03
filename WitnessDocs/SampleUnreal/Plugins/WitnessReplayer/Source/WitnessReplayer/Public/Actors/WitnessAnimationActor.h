#pragma once

#include "WitnessAnimationActor.generated.h"

UCLASS()
class WITNESSREPLAYER_API AWitnessAnimationActor : public AWitnessActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float AnimationAt;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FVector EffectorPosition;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FRotator EffectorRotation;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	AActor* AttachedActor;
};
