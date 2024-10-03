#include "Actors/WitnessActorSpawner.h"

AWitnessActorSpawner::AWitnessActorSpawner()
{
 	PrimaryActorTick.bCanEverTick = true;

}

void AWitnessActorSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWitnessActorSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AWitnessActor* AWitnessActorSpawner::SpawnWitnessActor(FString InstanceName, TSubclassOf<AWitnessActor> WitnessActorClass)
{
	UWorld* World = GetWorld();
	if(!ensure(World != nullptr)) return nullptr;

	AWitnessActor* SpawnedActor = (AWitnessActor*)World->SpawnActor(WitnessActorClass);
	if(!ensure(SpawnedActor != nullptr)) return nullptr;
	SpawnedActor->InstanceName = InstanceName;
	return SpawnedActor;
}


