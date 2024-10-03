#include "Actors/WitnessActor.h"

AWitnessActor::AWitnessActor()
{
 	PrimaryActorTick.bCanEverTick = true;

}

void AWitnessActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWitnessActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AWitnessActor::TransformLocation(FVector Location)
{
	float PositionMultiplier = 100.0f;
	Location *= PositionMultiplier;
	return FVector(Location.X, Location.Z, Location.Y);
}