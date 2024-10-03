#include "Actors/WitnessEntityActor.h"

AWitnessEntityActor::AWitnessEntityActor()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SplineComponent"));
	SetRootComponent(StaticMeshComponent);
}

void AWitnessEntityActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWitnessEntityActor::SetGeometry(UStaticMesh* _Mesh)
{
	if(_Mesh != nullptr)
	{
		StaticMeshComponent->SetStaticMesh(_Mesh);
	}
}
