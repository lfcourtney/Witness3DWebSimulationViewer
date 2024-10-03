#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WitnessActor.generated.h"

UCLASS()
class WITNESSREPLAYER_API AWitnessActor : public AActor
{
	GENERATED_BODY()
	
public:
	
	AWitnessActor();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FString InstanceName;

protected:
	
	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual FVector TransformLocation(FVector Location);
	
};
