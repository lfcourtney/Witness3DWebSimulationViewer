#pragma once

#include "CoreMinimal.h"
#include "WitnessActor.h"
#include "GameFramework/Actor.h"
#include "WitnessActorSpawner.generated.h"

UCLASS()
class WITNESSREPLAYER_API AWitnessActorSpawner : public AActor
{
	GENERATED_BODY()
	
public:	

	AWitnessActorSpawner();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	AWitnessActor* SpawnWitnessActor(FString InstanceName, TSubclassOf<AWitnessActor> WitnessActorClass);

};
